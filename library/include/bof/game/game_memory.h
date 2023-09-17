#pragma once

#include <bof/game/game.h>

#include <bof/io/screen.h>

#include <stdlib.h>
#include <stdint.h>

#define BLOCK_SIZE 4194304

typedef struct {
    char data[BLOCK_SIZE];
} memory_block_t;

typedef struct {
    uint8_t* program_data;
    uint64_t program_size;

    uint64_t stack_start;
    uint64_t stack_size;
    uint64_t* stack_data;

    uint64_t framebuffer_start;
    screen_buffer_t* framebuffer_data;

    uint64_t heap_start;
    memory_block_t** blocks;
    uint64_t block_count;
} memory_pool_t;

void expand_pool(memory_pool_t* pool, int blocks);

void write_data(memory_pool_t* pool, uint64_t address, void* buffer, size_t size);
void read_data(memory_pool_t* pool, uint64_t address, void* buffer, size_t size);

void init_pool(memory_pool_t* pool, game_program_t* program);
void destroy_pool(memory_pool_t* pool);

int get_total_memory(memory_pool_t* pool);
