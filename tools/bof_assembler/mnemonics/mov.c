#include <mnemonics.h>

define_mnemonic(MOV) {
    if (line->operand_num != 2) {
        throw_error("Only 2 operands are allowed");
    }

    operand_t* op1 = &line->operands[0];
    operand_t* op2 = &line->operands[1];

    //Number -> register
    // mov r1, 0x12345678
    if (op1->type == REGISTER_OPERAND && op1->indirect == false && op2->type == IMMEDIATE_OPERAND) {
        append_opcode(0x10);
        append_opcode(*((cpu_register_t*) op1->value));
        
        union register_value_t {
            uint64_t value;
            uint8_t bytes[8];
        };
        union register_value_t reg_val;
        reg_val.value = *((uint64_t*) op2->value);

        for (int i = 0; i < 8; i++) {
            append_opcode(reg_val.bytes[i]);
        }
    }
    //Register -> register
    // mov r1, r2
    else if (op1->type == REGISTER_OPERAND && op1->indirect == false && op2->type == REGISTER_OPERAND && op2->indirect == false) {
        append_opcode(0x11);
        append_opcode(*((cpu_register_t*) op1->value));
        append_opcode(*((cpu_register_t*) op2->value));
    }
    //Register -> address in register
    // mov [r1], r2
    else if (op1->type == REGISTER_OPERAND && op1->indirect && op2->type == REGISTER_OPERAND && op2->indirect == false) {
        append_opcode(0x12);
        append_opcode(*((cpu_register_t*) op1->value));
        append_opcode(*((cpu_register_t*) op2->value));

        union disp_value_t {
            int32_t value;
            uint8_t bytes[4];
        };
        union disp_value_t disp_val;
        disp_val.value = op1->displacement;

        for (int i = 0; i < 4; i++) {
            append_opcode(disp_val.bytes[i]);
        }
    }
    //Address in register -> register
    // mov r1, [r2]
    else if (op1->type == REGISTER_OPERAND && op1->indirect == false && op2->type == REGISTER_OPERAND && op2->indirect) {
        append_opcode(0x13);
        append_opcode(*((cpu_register_t*) op1->value));
        append_opcode(*((cpu_register_t*) op2->value));

        union disp_value_t {
            int32_t value;
            uint8_t bytes[4];
        };
        union disp_value_t disp_val;
        disp_val.value = op2->displacement;

        for (int i = 0; i < 4; i++) {
            append_opcode(disp_val.bytes[i]);
        }
    }
    //Label address -> register
    // mov r1, $label
    else if (op1->type == REGISTER_OPERAND && op1->indirect == false && op2->type == LABEL_OPERAND) {
        append_opcode(0x10);
        append_opcode(*((cpu_register_t*) op1->value));

        char* label_name = (char*) op2->value;
        append_label_reference(label_name);
    } else {
        throw_error("Invalid operands, MOV instruction was not understood");
    }
}
