#pragma once

#include <bof/game/game.h>
#include <bof/game/game_memory.h>

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

#define REGISTERS_COUNT 10
typedef union {
    uint64_t raw[REGISTERS_COUNT];
    struct {
        uint64_t r1; //General purpose registers
        uint64_t r2;
        uint64_t r3;
        uint64_t r4;
        uint64_t r5;
        uint64_t r6;
        uint64_t r7;
        uint64_t r8;

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

    uint64_t game_loop_address;
} game_vm_t;


void vm_init(game_vm_t* vm, game_program_t* program);
void vm_destroy(game_vm_t* vm);
void vm_register_dump(game_vm_t* vm);

void vm_stack_push(game_vm_t* vm, uint64_t value);
uint64_t vm_stack_pop(game_vm_t* vm);

//Run the program at the given address until it returns
void vm_run(game_vm_t* vm, uint64_t address);
