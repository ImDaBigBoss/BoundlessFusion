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
    vm->registers.sp--;

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
    
    uint64_t stack_address = vm->registers.sp - vm->memory_pool.stack_start;
    vm->registers.sp++;

    return vm->memory_pool.stack_data[stack_address];
}


#define CHECK_ADDRESS_BOUNDS(address) \
    if (address > vm->memory_pool.program_size) { \
        debug_error("Attempted to run program outside of reserved memory\n"); \
        lib_exit(3); \
    }

void vm_run(game_vm_t* vm, uint64_t address) {
    CHECK_ADDRESS_BOUNDS(address)

    vm->registers.pc = address;
    int running_functions = 1;

    while (running_functions != 0) {
        uint8_t opcode = vm->memory_pool.program_data[vm->registers.pc];
        debug_info("Running opcode 0x%x at address 0x%x", opcode, vm->registers.pc);
        vm->registers.pc++;

        switch (opcode) {
            case 0x00: {
                asm("nop");
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
    }
}