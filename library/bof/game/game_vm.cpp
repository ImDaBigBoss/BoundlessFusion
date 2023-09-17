#include <bof/game/game_vm.h>

#include <bof/io/debug.h>

#include <bof/main.h>

#include <stdlib.h>
#include <string.h>

void vm_init(game_vm_t* vm, game_program_t* program) {
    memset(vm, 0, sizeof(game_vm_t)); //This will set the registers to 0

    //Allocate memory pool
    init_pool(&vm->memory_pool, program);

    //Stack start addess: vm->memory_pool.stack_start vm->memory_pool.stack_size
    vm->registers.sp = vm->memory_pool.stack_start + vm->memory_pool.stack_size;
}

void vm_destroy(game_vm_t* vm) {
    destroy_pool(&vm->memory_pool);   
}

void vm_register_dump(game_vm_t* vm) {
    debug_raw("R1 = 0x%x, R2 = 0x%x\nR3 = 0x%x, R4 = 0x%x\nR5 = 0x%x, R6 = 0x%x\nR7 = 0x%x, R8 = 0x%x\nSP = 0x%x, PC = 0x%x\nFLAGS = 0x%x\n\n", vm->registers.r1, vm->registers.r2, vm->registers.r3, vm->registers.r4, vm->registers.r5, vm->registers.r6, vm->registers.r7, vm->registers.r8, vm->registers.sp, vm->registers.pc, vm->flags.raw);
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
    
    uint64_t stack_address = vm->registers.sp - vm->memory_pool.stack_start;
    vm->registers.sp -= 8;

    vm->memory_pool.stack_data[stack_address] = value;
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
    
    vm->registers.sp += 8;
    uint64_t stack_address = vm->registers.sp - vm->memory_pool.stack_start;

    return vm->memory_pool.stack_data[stack_address];
}


#define CHECK_ADDRESS_BOUNDS(address) \
    if (address > vm->memory_pool.program_size) { \
        debug_error("Attempted to run program outside of reserved memory\n"); \
        lib_exit(3); \
    }

#define READ_UINT8() \
    vm->memory_pool.program_data[vm->registers.pc++]

#define READ_UINT32() \
    *((uint32_t*) (vm->memory_pool.program_data + vm->registers.pc)); \
    vm->registers.pc += 4

#define READ_UINT64() \
    *((uint64_t*) (vm->memory_pool.program_data + vm->registers.pc)); \
    vm->registers.pc += 8

#define JUMP_FUNCTION(condition, before_jump) \
    uint8_t mode = READ_UINT8(); \
    uint64_t address; \
    \
    if (mode == 1) { /*Immediate*/ \
        address = READ_UINT64(); \
    } else if (mode == 2) { /*Register*/ \
        uint8_t reg = READ_UINT8() - 1; \
        address = vm->registers.raw[reg]; \
    } else { \
        debug_error("Invalid branching mode %d", mode); \
        lib_exit(3); \
        return; /*Not required, but removes compiler warnings*/ \
    } \
    before_jump \
    \
    if (condition) { \
        CHECK_ADDRESS_BOUNDS(address) \
        vm->registers.pc = address; \
    }

void vm_run(game_vm_t* vm, uint64_t address) {
    CHECK_ADDRESS_BOUNDS(address)

    vm->registers.pc = address;
    int running_functions = 1;

    //vm_register_dump(vm);

    while (running_functions != 0) {
        uint8_t opcode = READ_UINT8();
        //debug_info("Running opcode 0x%x at address 0x%x", opcode, vm->registers.pc);

        switch (opcode) {
            case 0x00: {
                asm("nop");
                break;
            }

            // --- Math ---

            case 0x01: { //Add
                uint8_t reg1 = READ_UINT8() - 1;
                uint8_t reg2 = READ_UINT8() - 1;

                vm->registers.raw[reg1] += vm->registers.raw[reg2];
                break;
            }
            case 0x02: { //Subtract
                uint8_t reg1 = READ_UINT8() - 1;
                uint8_t reg2 = READ_UINT8() - 1;

                vm->registers.raw[reg1] -= vm->registers.raw[reg2];
                break;
            }
            case 0x03: { //Multiply
                uint8_t reg1 = READ_UINT8() - 1;
                uint8_t reg2 = READ_UINT8() - 1;

                vm->registers.raw[reg1] *= vm->registers.raw[reg2];
                break;
            }
            case 0x04: { //Divide
                uint8_t reg1 = READ_UINT8() - 1;
                uint8_t reg2 = READ_UINT8() - 1;

                vm->registers.raw[reg1] /= vm->registers.raw[reg2];
                break;
            }
            case 0x05: { //Modulo
                uint8_t reg1 = READ_UINT8() - 1;
                uint8_t reg2 = READ_UINT8() - 1;

                vm->registers.raw[reg1] %= vm->registers.raw[reg2];
                break;
            }
            case 0x06: { //Increment
                uint8_t reg = READ_UINT8() - 1;

                vm->registers.raw[reg]++;
                break;
            }
            case 0x07: { //Decrement
                uint8_t reg = READ_UINT8() - 1;

                vm->registers.raw[reg]--;
                break;
            }
            case 0x08: { //Bitwise and
                uint8_t reg1 = READ_UINT8() - 1;
                uint8_t reg2 = READ_UINT8() - 1;

                vm->registers.raw[reg1] &= vm->registers.raw[reg2];
                break;
            }
            case 0x09: { //Bitwise or
                uint8_t reg1 = READ_UINT8() - 1;
                uint8_t reg2 = READ_UINT8() - 1;

                vm->registers.raw[reg1] |= vm->registers.raw[reg2];
                break;
            }
            case 0x0A: { //Shift left
                uint8_t reg = READ_UINT8() - 1;
                uint8_t amount = READ_UINT8();

                vm->registers.raw[reg] <<= amount;
                break;
            }
            case 0x0B: { //Shift right
                uint8_t reg = READ_UINT8() - 1;
                uint8_t amount = READ_UINT8();

                vm->registers.raw[reg] >>= amount;
                break;
            }

            // --- Memory ---
            case 0x10: { //register <- value
                uint8_t reg = READ_UINT8() - 1;
                uint64_t value = READ_UINT64();

                vm->registers.raw[reg] = value;
                break;
            }
            case 0x11: { //register <- register
                uint8_t reg1 = READ_UINT8() - 1;
                uint8_t reg2 = READ_UINT8() - 1;

                vm->registers.raw[reg1] = vm->registers.raw[reg2];
                break;
            }
            case 0x12: { //[register] <- register
                uint8_t reg1 = READ_UINT8() - 1;
                uint8_t reg2 = READ_UINT8() - 1;
                int32_t displacement = (int32_t) READ_UINT32();

                uint64_t address = vm->registers.raw[reg1] + displacement;
                write_data(&vm->memory_pool, address, (void*) &vm->registers.raw[reg2], sizeof(uint64_t));

                break;
            }
            case 0x13: { //register <- [register]
                uint8_t reg1 = READ_UINT8() - 1;
                uint8_t reg2 = READ_UINT8() - 1;
                int32_t displacement = (int32_t) READ_UINT32();

                uint64_t address = vm->registers.raw[reg2] + displacement;
                read_data(&vm->memory_pool, address, (void*) &vm->registers.raw[reg1], sizeof(uint64_t));

                break;
            }

            // --- Branching ---
            case 0x20: { //Call
                JUMP_FUNCTION(true, vm_stack_push(vm, vm->registers.pc + 9);)
                running_functions++;
                break;
            }
            case 0x21: { //Return
                running_functions--;
                if (running_functions == 0) {
                    vm->registers.pc = UINT64_MAX;
                    goto ret_reached;
                }

                uint64_t address = vm_stack_pop(vm);
                CHECK_ADDRESS_BOUNDS(address)

                vm->registers.pc = address;
                break;
            }
            case 0x22: { //Jump
                JUMP_FUNCTION(true,)
                break;
            }
            case 0x23: { //Jump equal
                JUMP_FUNCTION(vm->flags.zero,)
                break;
            }
            case 0x24: { //Jump not equal
                JUMP_FUNCTION(!vm->flags.zero,)
                break;
            }
            case 0x25: { //Jump greater than
                JUMP_FUNCTION(!vm->flags.zero && !vm->flags.negative,)
                break;
            }
            case 0x26: { //Jump greater than or equal
                JUMP_FUNCTION(vm->flags.zero || !vm->flags.negative,)
                break;
            }
            case 0x27: { //Jump less than
                JUMP_FUNCTION(vm->flags.negative,)
                break;
            }
            case 0x28: { //Jump less than or equal
                JUMP_FUNCTION(vm->flags.zero || vm->flags.negative,)
                break;
            }

            // --- Misc ---
            case 0x30: { //Compare registers
                int64_t value1 = (int64_t) vm->registers.raw[READ_UINT8() - 1];
                int64_t value2 = (int64_t) vm->registers.raw[READ_UINT8() - 1];

                int64_t result = value1 - value2;
                vm->flags.zero = result == 0;
                vm->flags.negative = result < 0;
                vm->flags.carry = value1 < value2;
                vm->flags.overflow = (value1 < 0 && value2 >= 0 && result >= 0) || (value1 >= 0 && value2 < 0 && result < 0);

                break;
            }
            case 0x31: { //Compare register to immediate
                int64_t value1 = (int64_t) vm->registers.raw[READ_UINT8() - 1];
                int64_t value2 = (int64_t) READ_UINT64();

                int64_t result = value1 - value2;
                vm->flags.zero = result == 0;
                vm->flags.negative = result < 0;
                vm->flags.carry = value1 < value2;
                vm->flags.overflow = (value1 < 0 && value2 >= 0 && result >= 0) || (value1 >= 0 && value2 < 0 && result < 0);
                break;
            }
            case 0x32: { //Syscall
                uint64_t syscall_id = READ_UINT64();
                debug_warn("Unhandled syscall %d", syscall_id);
                break;
            }

            // --- Stack ---
            case 0x40: { //Push
                uint8_t reg = READ_UINT8() - 1;
                vm_stack_push(vm, vm->registers.raw[reg]);
                break;
            }
            case 0x41: { //Pop
                uint8_t reg = READ_UINT8() - 1;
                vm->registers.raw[reg] = vm_stack_pop(vm);
                break;
            }
            case 0x42: { //Pushf
                vm_stack_push(vm, vm->flags.raw);
                break;
            }
            case 0x43: { //Popf
                vm->flags.raw = vm_stack_pop(vm);
                break;
            }
            case 0x44: { //Pusha
                for (int i = 0; i < REGISTERS_COUNT - 2; i++) { //We don't want to store SP and PC
                    vm_stack_push(vm, vm->registers.raw[i]);
                }
                break;
            }
            case 0x45: { //Popa
                for (int i = REGISTERS_COUNT - 3; i >= 0; i--) { //We don't want to store SP and PC
                    vm->registers.raw[i] = vm_stack_pop(vm);
                }
                break;
            }

            case 0xFF: {
                debug_info("Halt instruction reached, exiting...");
                lib_exit(0);
                break;
            }
            default: {
                debug_error("Invalid opcode 0x%x at address 0x%x", opcode, vm->registers.pc);
                lib_exit(3);
                break;
            }
        }

        //vm_register_dump(vm);
    }

    ret_reached:
    debug_info("Finished executing program section.");
}