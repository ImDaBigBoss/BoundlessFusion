#pragma once

#include <bof/game/game.h>

#include <bof/game/game_memory.h>

#define REGISTER_COUNT 6
#define STACK_SIZE (4096 * 1000) //4 MB

typedef union {
    uint64_t raw;
    struct {
        uint64_t zero : 1;
        uint64_t carry : 1;
        uint64_t negative : 1;
        uint64_t overflow : 1;
        uint64_t unused : 60;
    };
} flags_t;

typedef union {
    uint64_t raw[REGISTER_COUNT];
    struct {
        uint64_t r1; //General purpose registers
        uint64_t r2;
        uint64_t r3;
        uint64_t r4;
        uint64_t sp; //Stack pointer
        uint64_t pc; //Program counter
    };
} registers_t;


typedef struct {
    //Registers
    registers_t registers;
    flags_t flags; //technically a register, but not accessible directly

    //Memory
    memory_pool_t memory_pool;
} game_vm_t;

void vm_init(game_vm_t* vm, game_program_t* program);
void vm_destroy(game_vm_t* vm);
