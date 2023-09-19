#include <bof/game/game_opcodes.h>
#include <bof/game/syscalls.h>

#include <bof/io/debug.h>

#include <bof/main.h>

#include <string.h>

vm_opcode_t vm_opcodes[256];

#define REGISTER_OPCODE(id, function) \
    vm_opcodes[id] = &function

void invalid_opcode(game_vm_t* vm, int* running_functions) {
    vm->registers.pc -= 1;
    uint8_t opcode = READ_UINT8();

    debug_error("Invalid opcode 0x%x at address 0x%llx", opcode, vm->registers.pc);
    lib_exit(3);
}

void opcode_nop(game_vm_t* vm, int* running_functions) {
    asm("nop");
}

// --- Math ---
void opcode_add(game_vm_t* vm, int* running_functions) {
    uint8_t reg1 = READ_UINT8() - 1;
    uint8_t reg2 = READ_UINT8() - 1;

    vm->registers.raw[reg1] += vm->registers.raw[reg2];
}
void opcode_sub(game_vm_t* vm, int* running_functions) {
    uint8_t reg1 = READ_UINT8() - 1;
    uint8_t reg2 = READ_UINT8() - 1;

    vm->registers.raw[reg1] -= vm->registers.raw[reg2];
}
void opcode_mul(game_vm_t* vm, int* running_functions) {
    uint8_t reg1 = READ_UINT8() - 1;
    uint8_t reg2 = READ_UINT8() - 1;

    vm->registers.raw[reg1] *= vm->registers.raw[reg2];
}
void opcode_div(game_vm_t* vm, int* running_functions) {
    uint8_t reg1 = READ_UINT8() - 1;
    uint8_t reg2 = READ_UINT8() - 1;

    vm->registers.raw[reg1] /= vm->registers.raw[reg2];
}
void opcode_mod(game_vm_t* vm, int* running_functions) {
    uint8_t reg1 = READ_UINT8() - 1;
    uint8_t reg2 = READ_UINT8() - 1;

    vm->registers.raw[reg1] %= vm->registers.raw[reg2];
}
void opcode_inc(game_vm_t* vm, int* running_functions) {
    uint8_t reg = READ_UINT8() - 1;

    vm->registers.raw[reg]++;
}
void opcode_dec(game_vm_t* vm, int* running_functions) {
    uint8_t reg = READ_UINT8() - 1;

    vm->registers.raw[reg]--;
}
void opcode_and(game_vm_t* vm, int* running_functions) {
    uint8_t reg1 = READ_UINT8() - 1;
    uint8_t reg2 = READ_UINT8() - 1;

    vm->registers.raw[reg1] &= vm->registers.raw[reg2];
}
void opcode_or(game_vm_t* vm, int* running_functions) {
    uint8_t reg1 = READ_UINT8() - 1;
    uint8_t reg2 = READ_UINT8() - 1;

    vm->registers.raw[reg1] |= vm->registers.raw[reg2];
}
void opcode_shl(game_vm_t* vm, int* running_functions) {
    uint8_t reg = READ_UINT8() - 1;
    uint8_t amount = READ_UINT8();

    vm->registers.raw[reg] <<= amount;
}
void opcode_shr(game_vm_t* vm, int* running_functions) {
    uint8_t reg = READ_UINT8() - 1;
    uint8_t amount = READ_UINT8();

    vm->registers.raw[reg] >>= amount;
}

// --- Memory ---
void opcode_mov(game_vm_t* vm, int* running_functions) { //register <- value
    uint8_t reg = READ_UINT8() - 1;
    uint64_t value = READ_UINT64();

    vm->registers.raw[reg] = value;
}
void opcode_movr(game_vm_t* vm, int* running_functions) { //register <- register
    uint8_t reg1 = READ_UINT8() - 1;
    uint8_t reg2 = READ_UINT8() - 1;

    vm->registers.raw[reg1] = vm->registers.raw[reg2];
}
void opcode_stor(game_vm_t* vm, int* running_functions) { //[register] <- register
    uint8_t reg1 = READ_UINT8() - 1;
    uint8_t reg2 = READ_UINT8() - 1;
    int32_t displacement = (int32_t) READ_UINT32();

    uint64_t address = vm->registers.raw[reg1] + displacement;
    write_data(&vm->memory_pool, address, (void*) &vm->registers.raw[reg2], sizeof(uint64_t));

}
void opcode_load(game_vm_t* vm, int* running_functions) { //register <- [register]
    uint8_t reg1 = READ_UINT8() - 1;
    uint8_t reg2 = READ_UINT8() - 1;
    int32_t displacement = (int32_t) READ_UINT32();

    uint64_t address = vm->registers.raw[reg2] + displacement;
    read_data(&vm->memory_pool, address, (void*) &vm->registers.raw[reg1], sizeof(uint64_t));

}

// --- Branching ---
void opcode_call(game_vm_t* vm, int* running_functions) {
    JUMP_FUNCTION(true, vm_stack_push(vm, vm->registers.pc + 9);)
    running_functions++;
}
void opcode_ret(game_vm_t* vm, int* running_functions) {
    if (--*running_functions == 0) {
        vm->registers.pc = UINT64_MAX;
        return;
    }

    uint64_t address = vm_stack_pop(vm);
    CHECK_ADDRESS_BOUNDS(address)

    vm->registers.pc = address;
}
void opcode_jmp(game_vm_t* vm, int* running_functions) {
    JUMP_FUNCTION(true,)
}
void opcode_je(game_vm_t* vm, int* running_functions) {
    JUMP_FUNCTION(vm->flags.zero,)
}
void opcode_jne(game_vm_t* vm, int* running_functions) {
    JUMP_FUNCTION(!vm->flags.zero,)
}
void opcode_jg(game_vm_t* vm, int* running_functions) {
    JUMP_FUNCTION(!vm->flags.zero && !vm->flags.negative,)
}
void opcode_jge(game_vm_t* vm, int* running_functions) {
    JUMP_FUNCTION(vm->flags.zero || !vm->flags.negative,)
}
void opcode_jl(game_vm_t* vm, int* running_functions) {
    JUMP_FUNCTION(vm->flags.negative,)
}
void opcode_jle(game_vm_t* vm, int* running_functions) {
    JUMP_FUNCTION(vm->flags.zero || vm->flags.negative,)
}

// --- Misc ---
void opcode_cmp(game_vm_t* vm, int* running_functions) {
    int64_t value1 = (int64_t) vm->registers.raw[READ_UINT8() - 1];
    int64_t value2 = (int64_t) vm->registers.raw[READ_UINT8() - 1];

    int64_t result = value1 - value2;
    vm->flags.zero = result == 0;
    vm->flags.negative = result < 0;
    vm->flags.carry = value1 < value2;
    vm->flags.overflow = ((value1 < 0) != (value2 < 0)) && ((value1 < 0) != (result < 0));
}
void opcode_cmpi(game_vm_t* vm, int* running_functions) {
    int64_t value1 = (int64_t) vm->registers.raw[READ_UINT8() - 1];
    int64_t value2 = (int64_t) READ_UINT64();

    int64_t result = value1 - value2;
    vm->flags.zero = result == 0;
    vm->flags.negative = result < 0;
    vm->flags.carry = value1 < value2;
    vm->flags.overflow = ((value1 < 0) != (value2 < 0)) && ((value1 < 0) != (result < 0));
}
void opcode_syscall(game_vm_t* vm, int* running_functions) {
    uint64_t syscall_id = READ_UINT64();
    if (syscall_id == 0 || syscall_id > SYSCALL_COUNT) {
        debug_error("Invalid syscall id %d", syscall_id);
        lib_exit(3);
    }

    vm_syscalls[syscall_id - 1](vm);
}

// --- Stack ---
void opcode_push(game_vm_t* vm, int* running_functions) {
    uint8_t reg = READ_UINT8() - 1;
    vm_stack_push(vm, vm->registers.raw[reg]);
}
void opcode_pop(game_vm_t* vm, int* running_functions) {
    uint8_t reg = READ_UINT8() - 1;
    vm->registers.raw[reg] = vm_stack_pop(vm);
}
void opcode_pushf(game_vm_t* vm, int* running_functions) {
    vm_stack_push(vm, vm->flags.raw);
}
void opcode_popf(game_vm_t* vm, int* running_functions) {
    vm->flags.raw = vm_stack_pop(vm);
}
void opcode_pusha(game_vm_t* vm, int* running_functions) {
    for (int i = 0; i < REGISTERS_COUNT - 2; i++) { //We don't want to store SP and PC
        vm_stack_push(vm, vm->registers.raw[i]);
    }
}
void opcode_popa(game_vm_t* vm, int* running_functions) {
    for (int i = REGISTERS_COUNT - 3; i >= 0; i--) { //We don't want to store SP and PC
        vm->registers.raw[i] = vm_stack_pop(vm);
    }
}

void opcode_hlt(game_vm_t* vm, int* running_functions) {
    debug_info("Halt instruction reached, exiting...");
    lib_exit(0);
}

void register_opcodes() {
    memset(vm_opcodes, 0, sizeof(vm_opcodes));

    REGISTER_OPCODE(0x00, opcode_nop);

    // --- Math ---
    REGISTER_OPCODE(0x01, opcode_add);
    REGISTER_OPCODE(0x02, opcode_sub);
    REGISTER_OPCODE(0x03, opcode_mul);
    REGISTER_OPCODE(0x04, opcode_div);
    REGISTER_OPCODE(0x05, opcode_mod);
    REGISTER_OPCODE(0x06, opcode_inc);
    REGISTER_OPCODE(0x07, opcode_dec);
    REGISTER_OPCODE(0x08, opcode_and);
    REGISTER_OPCODE(0x09, opcode_or);
    REGISTER_OPCODE(0x0A, opcode_shl);
    REGISTER_OPCODE(0x0B, opcode_shr);

    // --- Memory ---
    REGISTER_OPCODE(0x10, opcode_mov);
    REGISTER_OPCODE(0x11, opcode_movr);
    REGISTER_OPCODE(0x12, opcode_stor);
    REGISTER_OPCODE(0x13, opcode_load);

    // --- Branching ---
    REGISTER_OPCODE(0x20, opcode_call);
    REGISTER_OPCODE(0x21, opcode_ret);
    REGISTER_OPCODE(0x22, opcode_jmp);
    REGISTER_OPCODE(0x23, opcode_je);
    REGISTER_OPCODE(0x24, opcode_jne);
    REGISTER_OPCODE(0x25, opcode_jg);
    REGISTER_OPCODE(0x26, opcode_jge);
    REGISTER_OPCODE(0x27, opcode_jl);
    REGISTER_OPCODE(0x28, opcode_jle);

    // --- Misc ---
    REGISTER_OPCODE(0x30, opcode_cmp);
    REGISTER_OPCODE(0x31, opcode_cmpi);
    REGISTER_OPCODE(0x32, opcode_syscall);

    // --- Stack ---
    REGISTER_OPCODE(0x40, opcode_push);
    REGISTER_OPCODE(0x41, opcode_pop);
    REGISTER_OPCODE(0x42, opcode_pushf);
    REGISTER_OPCODE(0x43, opcode_popf);
    REGISTER_OPCODE(0x44, opcode_pusha);
    REGISTER_OPCODE(0x45, opcode_popa);

    REGISTER_OPCODE(0xFF, opcode_hlt);

    for (int i = 0; i < 256; i++) {
        if (vm_opcodes[i] == 0) {
            vm_opcodes[i] = &invalid_opcode;
        }
    }
}
