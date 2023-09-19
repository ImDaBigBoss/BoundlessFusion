#pragma once

#include <bof/game/game_vm.h>

#define CHECK_ADDRESS_BOUNDS(address) \
    if (address > vm->memory_pool.program_size) { \
        debug_error("Attempted to run program outside of reserved memory"); \
        lib_exit(3); \
    }

#define READ_UINT8() \
    vm->memory_pool.program_data[vm->registers.pc++]

#define READ_UINT32() \
    *((uint32_t*) (((uint64_t) vm->memory_pool.program_data) + vm->registers.pc)); \
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

typedef void (*vm_opcode_t)(game_vm_t* vm, int* running_functions);
extern vm_opcode_t vm_opcodes[256];

void register_opcodes();
