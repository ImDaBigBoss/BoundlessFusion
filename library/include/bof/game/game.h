#pragma once

#include <stdint.h>

#define GAME_MAGIC 0x424F4647 //BOFG

typedef struct {
    uint32_t magic;
    uint8_t version;
    uint32_t size;
    uint64_t start_address;
} game_file_header_t;

typedef struct {
    void* data;
    uint64_t size;
    uint64_t start_address;
} game_program_t;
