#include <bof/game/game_memory.h>

#include <bof/io/debug.h>

#include <bof/main.h>

#include <stdlib.h>
#include <string.h>

//Functions

void expand_pool(memory_pool_t* pool, int blocks) {
    if (blocks <= 0) {
        debug_error("Invalid block count\n");
        lib_exit(2);
    }

    pool->block_count += blocks;
    if (pool->blocks) {
        pool->blocks = (memory_block_t**) realloc(pool->blocks, pool->block_count * sizeof(memory_block_t*));
    } else {
        pool->blocks = (memory_block_t**) malloc(pool->block_count * sizeof(memory_block_t*));
    }
    if (pool->blocks == NULL) {
        debug_error("Failed to allocate new block index\n");
        lib_exit(2);
    }

    for (int i = pool->block_count - blocks; i < pool->block_count; i++) {
        void* new_block = (memory_block_t*) malloc(sizeof(memory_block_t));
        if (new_block == NULL) {
            debug_error("Failed to allocate new block\n");
            lib_exit(2);
        }
        memset(new_block, 0, sizeof(memory_block_t));

        pool->blocks[i] = (memory_block_t*) new_block;
    }
}


void write_data(memory_pool_t* pool, uint64_t address, void* buffer, size_t size) {
    if (address < pool->heap_start) {
        if (address < pool->stack_start) {
            debug_error("Attempted to write to reserved memory located before the stack\n");
            lib_exit(2);
        } else {
            uint64_t stack_address = address - pool->stack_start;

            if (stack_address + size > pool->stack_size) {
                debug_error("Attempted to write to stack outside of reserved memory\n");
                lib_exit(2);
            }

            memcpy((void*) ((uint64_t) pool->stack + stack_address), buffer, size);
        }

        return;
    }

    size_t written = 0;
    uint64_t current_address = address - pool->heap_start; //Get the address relative to the heap start

    while (written < size) {
        uint64_t block_id = current_address / BLOCK_SIZE;
        uint64_t block_offset = current_address % BLOCK_SIZE;

        if (block_id >= pool->block_count) {
            debug_error("Invalid block ID\n");
            lib_exit(2);
        }

        memory_block_t* block = pool->blocks[block_id];
        size_t can_write = BLOCK_SIZE - block_offset;
        size_t to_write = size - written;

        if (to_write > can_write) {
            to_write = can_write;
        }

        memcpy(block->data + block_offset, (void*) ((uint64_t) buffer + written), to_write);
        written += to_write;
        current_address += to_write;
    }
}

void read_data(memory_pool_t* pool, uint64_t address, void* buffer, size_t size) {
    if (address < pool->heap_start) {
        if (address < pool->program_size) {
            if (address + size > pool->program_size) {
                debug_error("Attempted to read from program data outside of reserved memory\n");
                lib_exit(2);
            }

            memcpy(buffer, (void*) ((uint64_t) pool->program_data + address), size);
        } else if (address < pool->stack_start) {
            debug_error("Attempted to read from reserved memory\n");
            lib_exit(2);
        } else {
            uint64_t stack_address = address - pool->stack_start;

            if (stack_address + size > pool->stack_size) {
                debug_error("Attempted to read from stack outside of reserved memory\n");
                lib_exit(2);
            }

            memcpy(buffer, (void*) ((uint64_t) pool->stack + stack_address), size);
        }

        return;
    }

    size_t read = 0;
    uint64_t current_address = address - pool->heap_start; //Get the address relative to the heap start

    while (read < size) {
        uint64_t block_id = current_address / BLOCK_SIZE;
        uint64_t block_offset = current_address % BLOCK_SIZE;

        if (block_id >= pool->block_count) {
            debug_error("Invalid block ID\n");
            lib_exit(2);
        }

        memory_block_t* block = pool->blocks[block_id];
        size_t can_read = BLOCK_SIZE - block_offset;
        size_t to_read = size - read;

        if (to_read > can_read) {
            to_read = can_read;
        }

        memcpy((void*) ((uint64_t) buffer + read), block->data + block_offset, to_read);
        read += to_read;
        current_address += to_read;
    }
}


void init_pool(memory_pool_t* pool, game_program_t* program) {
    debug_info("Initializing memory pool\n");
    memset(pool, 0, sizeof(memory_pool_t));

    //Program data starts at 0
    pool->program_data = program->data;
    pool->program_size = program->size;

    //Stack
    pool->stack_start = program->size + 10; //10 bytes of padding
    if (pool->stack_start % 0x10 > 0) { //Align to 16 bytes
        pool->stack_start += 0x10 - (pool->stack_start % 0x10);
    }

    pool->stack_size = BLOCK_SIZE;
    pool->stack = malloc(pool->stack_size);

    //Heap
    pool->heap_start = (pool->stack_start + pool->stack_size) + 10; //10 bytes of padding
    if (pool->heap_start % 0x10 > 0) { //Align to 16 bytes
        pool->heap_start += 0x10 - (pool->heap_start % 0x10);
    }
    expand_pool(pool, 2);
}

void destroy_pool(memory_pool_t* pool) {
    debug_info("Destroying memory pool\n");

    free(pool->program_data);
    free(pool->stack);

    for (int i = 0; i < pool->block_count; i++) {
        free(pool->blocks[i]);
    }
    free(pool->blocks);
}


int get_total_memory(memory_pool_t* pool) {
    return pool->block_count * sizeof(memory_block_t);
}
