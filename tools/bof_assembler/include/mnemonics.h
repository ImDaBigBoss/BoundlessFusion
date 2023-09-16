#pragma once

#include <asmutils.h>

#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
    char* name;
    int local_address;
} label_t;

#define define_mnemonic(name) \
    void mnemonic_##name(lvl2_line_t* line, uint8_t** opcodes, int* opcode_num, label_t** label_references, int* label_reference_num, bool* errors_found)

#define check_mnemonic(name) \
    else if (strcmp(line->mnemonic, #name) == 0) { \
        mnemonic_##name(line, opcodes, opcode_num, &label_references, &label_reference_num, &errors_found); \
    }

#define throw_error(message, ...) \
    printf("Error line %d: "message"\n", line->line_num, ##__VA_ARGS__); \
    *errors_found = true; \
    return;

#define append_opcode(op) \
    *opcodes = realloc(*opcodes, sizeof(uint8_t) * (*opcode_num + 1)); \
    (*opcodes)[*opcode_num] = op; \
    (*opcode_num)++;

#define append_label_reference(label_name) \
    *label_references = realloc(*label_references, sizeof(label_t) * (*label_reference_num + 1)); \
    (*label_references)[*label_reference_num].name = malloc(strlen(label_name) + 1); \
    strcpy((*label_references)[*label_reference_num].name, label_name); \
    (*label_references)[*label_reference_num].local_address = *opcode_num; \
    (*label_reference_num)++; \
    for (int i = 0; i < 8; i++) { \
        append_opcode(0x00); \
    }

//NOP is done in the assembler

define_mnemonic(ADD);
define_mnemonic(SUB);
define_mnemonic(MUL);
define_mnemonic(DIV);
define_mnemonic(MOD);
define_mnemonic(INC);
define_mnemonic(DEC);

define_mnemonic(AND);
define_mnemonic(OR);
define_mnemonic(SHL);
define_mnemonic(SHR);

define_mnemonic(MOV);

define_mnemonic(CALL);
define_mnemonic(RET);
define_mnemonic(JMP);
define_mnemonic(JE);
define_mnemonic(JNE);
define_mnemonic(JG);
define_mnemonic(JGE);
define_mnemonic(JL);
define_mnemonic(JLE);

define_mnemonic(CMP);
define_mnemonic(SYSCALL);

define_mnemonic(PUSH);
define_mnemonic(POP);
define_mnemonic(PUSHF);
define_mnemonic(POPF);
define_mnemonic(PUSHA);
define_mnemonic(POPA);

define_mnemonic(HLT);

//No opcodes
define_mnemonic(DB);
define_mnemonic(DW);
define_mnemonic(DDW);
define_mnemonic(DQW);
