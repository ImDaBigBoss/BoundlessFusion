#include <bof/game/game_vm.h>
#include <bof/game/game_opcodes.h>
#include <bof/game/syscalls.h>

#include <bof/io/debug.h>

#include <bof/main.h>

#include <stdlib.h>
#include <string.h>

void vm_init(game_vm_t* vm, game_program_t* program) {
    memset(vm, 0, sizeof(game_vm_t)); //This will set the registers to 0
    vm->game_loop_address = program->start_address;

    //Allocate memory pool
    init_pool(&vm->memory_pool, program);
    register_syscalls();

    //Stack start addess: vm->memory_pool.stack_start vm->memory_pool.stack_size
    vm->registers.sp = vm->memory_pool.stack_start + vm->memory_pool.stack_size;

    register_opcodes();
}

void vm_destroy(game_vm_t* vm) {
    destroy_pool(&vm->memory_pool);   
}

void vm_register_dump(game_vm_t* vm) {
    debug_raw("R1 = 0x%016llx, R2 = 0x%016llx\nR3 = 0x%016llx, R4 = 0x%016llx\nR5 = 0x%016llx, R6 = 0x%016llx\nR7 = 0x%016llx, R8 = 0x%016llx\nSP = 0x%016llx, PC = 0x%016llx\nFLAGS = 0x%016llx\n\n", vm->registers.r1, vm->registers.r2, vm->registers.r3, vm->registers.r4, vm->registers.r5, vm->registers.r6, vm->registers.r7, vm->registers.r8, vm->registers.sp, vm->registers.pc, vm->flags.raw);
}


void vm_stack_push(game_vm_t* vm, uint64_t value) {
    if (vm->registers.sp < vm->memory_pool.stack_start) {
        debug_error("Stack overflow");
        lib_exit(2);
    }
    if (vm->registers.sp > vm->memory_pool.stack_start + vm->memory_pool.stack_size) {
        debug_error("Stack underflow");
        lib_exit(2);
    }

    vm->registers.sp -= sizeof(uint64_t);
    uint64_t* stack_address_ptr = (uint64_t*)((char*)vm->memory_pool.stack_data + (vm->registers.sp - vm->memory_pool.stack_start));
    *stack_address_ptr = value;
}

uint64_t vm_stack_pop(game_vm_t* vm) {
    if (vm->registers.sp < vm->memory_pool.stack_start) {
        debug_error("Stack overflow");
        lib_exit(2);
    }
    if (vm->registers.sp > vm->memory_pool.stack_start + vm->memory_pool.stack_size) {
        debug_error("Stack underflow");
        lib_exit(2);
    }

    uint64_t* stack_address_ptr = (uint64_t*)((char*)vm->memory_pool.stack_data + (vm->registers.sp - vm->memory_pool.stack_start));
    vm->registers.sp += sizeof(uint64_t);
    return *stack_address_ptr;
}


void vm_run(game_vm_t* vm, uint64_t address) {
    CHECK_ADDRESS_BOUNDS(address)

    vm->registers.pc = address;
    int running_functions = 1;

    //vm_register_dump(vm);

    while (running_functions != 0) {
        uint8_t opcode = READ_UINT8();
        //debug_info("Running opcode 0x%x at address 0x%x", opcode, vm->registers.pc);

        vm_opcodes[opcode](vm, &running_functions);

        //vm_register_dump(vm);
    }

    //debug_info("Finished executing program section.");
}