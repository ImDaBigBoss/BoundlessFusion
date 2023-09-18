#include <bof/game/game_memory.h>

#include <bof/io/debug.h>

#include <bof/main.h>

#include <stdlib.h>
#include <string.h>

uint64_t stack_buffer[BLOCK_SIZE / sizeof(uint64_t)]; //If this changes, update the size in the init function

//Functions

void expand_pool(memory_pool_t* pool, int blocks) {
    if (blocks <= 0) {
        debug_error("Invalid block count");
        lib_exit(2);
    }

    pool->block_count += blocks;
    if (pool->blocks) {
        pool->blocks = (memory_block_t**) realloc(pool->blocks, pool->block_count * sizeof(memory_block_t*));
    } else {
        pool->blocks = (memory_block_t**) malloc(pool->block_count * sizeof(memory_block_t*));
    }
    if (pool->blocks == NULL) {
        debug_error("Failed to allocate new block index");
        lib_exit(2);
    }

    pool->heap_size += blocks * BLOCK_SIZE;

    for (int i = pool->block_count - blocks; i < pool->block_count; i++) {
        void* new_block = (memory_block_t*) malloc(sizeof(memory_block_t));
        if (new_block == NULL) {
            debug_error("Failed to allocate new block");
            lib_exit(2);
        }
        memset(new_block, 0, sizeof(memory_block_t));

        pool->blocks[i] = (memory_block_t*) new_block;
    }
}


void write_data(memory_pool_t* pool, uint64_t address, void* buffer, size_t size) {
    if (address < pool->heap_start) {
        if (address < pool->program_size) {
            if (address + size > pool->program_size) {
                debug_error("Attempted to write from program data outside of reserved memory");
                lib_exit(2);
            }

            memcpy((void*) (pool->program_data + address), buffer, size);
        } else if (address < pool->framebuffer_start) {
            debug_error("Attempted to write to reserved memory");
            lib_exit(2);
        } else if (address < pool->framebuffer_size) {
            uint64_t framebuffer_address = address - pool->framebuffer_start;

            if (framebuffer_address + size > pool->framebuffer_size) {
                debug_error("Attempted to write to framebuffer outside of reserved memory");
                lib_exit(2);
            }

            memcpy((void*) (pool->framebuffer_data->data + framebuffer_address), buffer, size);
        } else if (address < pool->stack_start) {
            debug_error("Attempted to write to reserved memory");
            lib_exit(2);
        } else {
            uint64_t stack_address = address - pool->stack_start;

            if (stack_address + size > pool->stack_size) {
                debug_error("Attempted to write to stack outside of reserved memory");
                lib_exit(2);
            }

            memcpy((void*) (pool->stack_data + stack_address), buffer, size);
        }

        return;
    }

    size_t written = 0;
    uint64_t heap_address = address - pool->heap_start; //Get the address relative to the heap start

    if (heap_address + size > pool->heap_size) {
        debug_error("Attempted to write to heap outside of reserved memory");
        lib_exit(2);
    }

    while (written < size) {
        uint64_t block_id = heap_address / BLOCK_SIZE;
        uint64_t block_offset = heap_address % BLOCK_SIZE;

        if (block_id >= pool->block_count) {
            debug_error("Writing to invalid block ID at 0x%x", address);
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
        heap_address += to_write;
    }
}

void read_data(memory_pool_t* pool, uint64_t address, void* buffer, size_t size) {
    if (address < pool->heap_start) {
        if (address < pool->program_size) { //Program data
            if (address + size > pool->program_size) {
                debug_error("Attempted to read from program data outside of reserved memory");
                lib_exit(2);
            }

            memcpy(buffer, (void*) (pool->program_data + address), size);
        } else if (address < pool->framebuffer_start) {
            debug_error("Attempted to read from reserved memory");
            lib_exit(2);
        } else if (address < pool->framebuffer_size) {
            uint64_t framebuffer_address = address - pool->framebuffer_start;

            if (framebuffer_address + size > pool->framebuffer_size) {
                debug_error("Attempted to read from framebuffer outside of reserved memory");
                lib_exit(2);
            }

            memcpy(buffer, (void*) (pool->framebuffer_data->data + framebuffer_address), size);
        } else if (address < pool->stack_start) {
            debug_error("Attempted to read from reserved memory");
            lib_exit(2);
        } else {
            uint64_t stack_address = address - pool->stack_start;

            if (stack_address + size > pool->stack_size) {
                debug_error("Attempted to read from stack outside of reserved memory");
                lib_exit(2);
            }

            memcpy(buffer, (void*) (pool->stack_data + stack_address), size);
        }

        return;
    }

    size_t read = 0;
    uint64_t heap_address = address - pool->heap_start; //Get the address relative to the heap start

    if (heap_address + size > pool->heap_size) {
        debug_error("Attempted to read from heap outside of reserved memory");
        lib_exit(2);
    }

    while (read < size) {
        uint64_t block_id = heap_address / BLOCK_SIZE;
        uint64_t block_offset = heap_address % BLOCK_SIZE;

        if (block_id >= pool->block_count) {
            debug_error("Reading from invalid block ID at 0x%x", address);
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
        heap_address += to_read;
    }
}


void init_pool(memory_pool_t* pool, game_program_t* program) {
    debug_info("Initializing memory pool");
    memset(pool, 0, sizeof(memory_pool_t));

    //Program data starts at 0
    pool->program_data = (uint8_t*) program->data;
    pool->program_size = program->size;
    pool->program_data[program->size] = 0xFF; //Halt instruction (we reserved the last byte for this in game_loader.cpp)

    //Stack
    pool->stack_start = program->size + 32; //32 bytes of padding
    if (pool->stack_start % 0x10 > 0) { //Align to 16 bytes
        pool->stack_start += 0x10 - (pool->stack_start % 0x10);
    }
    pool->stack_size = sizeof(stack_buffer);
    pool->stack_data = stack_buffer;
    memset(pool->stack_data, 0, pool->stack_size);

    //Framebuffer
    pool->framebuffer_start = (pool->stack_start + pool->stack_size) + 32; //32 bytes of padding
    if (pool->framebuffer_start % 0x10 > 0) { //Align to 16 bytes
        pool->framebuffer_start += 0x10 - (pool->framebuffer_start % 0x10);
    }
    pool->framebuffer_size = SCREEN_MAX_WIDTH * SCREEN_MAX_HEIGHT * sizeof(uint32_t);
    //Framebuffer data setup by screen IO

    //Heap
    pool->heap_start = (pool->framebuffer_start + pool->framebuffer_size) + 10; //10 bytes of padding
    if (pool->heap_start % 0x10 > 0) { //Align to 16 bytes
        pool->heap_start += 0x10 - (pool->heap_start % 0x10);
    }
    expand_pool(pool, 2);
}

void destroy_pool(memory_pool_t* pool) {
    debug_info("Destroying memory pool");

    free(pool->program_data);

    for (int i = 0; i < pool->block_count; i++) {
        free(pool->blocks[i]);
    }
    free(pool->blocks);
}


int get_total_memory(memory_pool_t* pool) {
    return pool->block_count * sizeof(memory_block_t);
}
