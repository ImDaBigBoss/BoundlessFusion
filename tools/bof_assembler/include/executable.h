#pragma once

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define GAME_MAGIC 0x424F4647 //BOFG

typedef struct {
    uint32_t magic;
    uint8_t version;
    uint32_t size;
    uint64_t start_address;
} game_file_header_t;

void write_exec(FILE* file, uint8_t* opcodes, uint64_t opcode_num, int padding);
