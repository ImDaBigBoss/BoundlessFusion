#pragma once

#include <bof/game/game.h>

#include <bof/game/game_memory.h>

typedef union {
    volatile uint64_t raw;
    struct {
        volatile uint64_t zero : 1;
        volatile uint64_t carry : 1;
        volatile uint64_t negative : 1;
        volatile uint64_t overflow : 1;
        volatile uint64_t unused : 60;
    };
} flags_t;

typedef union {
    volatile uint64_t raw[6];
    struct {
        volatile uint64_t r1; //General purpose registers
        volatile uint64_t r2;
        volatile uint64_t r3;
        volatile uint64_t r4;

        volatile uint64_t sp; //Stack pointer
        volatile uint64_t pc; //Program counter
    };
} registers_t;


typedef struct {
    //Registers
    volatile registers_t registers;
    volatile flags_t flags; //technically a register, but not accessible directly

    //Memory
    memory_pool_t memory_pool;
} game_vm_t;

void vm_init(game_vm_t* vm, game_program_t* program);
void vm_destroy(game_vm_t* vm);
void vm_register_dump(game_vm_t* vm);

void vm_stack_push(game_vm_t* vm, uint64_t value);
uint64_t vm_stack_pop(game_vm_t* vm);

//Run the program at the given address until it returns
void vm_run(game_vm_t* vm, uint64_t address);
