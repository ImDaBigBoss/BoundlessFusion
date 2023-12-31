#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    UNDEFINED_OPERAND,
    REGISTER_OPERAND,
    STRING_OPERAND,
    IMMEDIATE_OPERAND,
    LABEL_OPERAND
} operand_type_t;

typedef struct {
    void* value;
    int size;
    bool indirect; //If true, the value is an address in a register
    int32_t displacement; //Used for indirect operands
    operand_type_t type;
} operand_t;

typedef struct {
    char* buffer;
    int buffer_size;

    int line_num;
    int part_num;

    char* mnemonic;
    char** operands;
} lvl1_line_t;

typedef struct {
    int line_num;
    char* mnemonic;
    int operand_num;
    operand_t* operands;
} lvl2_line_t;

typedef enum {
    R1 = 1,
    R2,
    R3,
    R4,
    R5,
    R6,
    R7,
    R8,

    SP, //Stack pointer
    PC, //Program counter
} cpu_register_t;

cpu_register_t get_register(char* str);

bool is_digits(char* str);

bool is_hex(char* str);

bool is_binary(char* str);

bool is_letters(char* str);

bool is_alphanumeric(char* str);

void str_to_upper(char* str);

uint64_t hex_str_to_num(char* str);

uint64_t binary_str_to_num(char* str);

uint64_t str_to_num(char* str);

char* find_string_end(char* str, char string_delimiter);