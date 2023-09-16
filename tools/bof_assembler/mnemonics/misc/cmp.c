#include <mnemonics.h>

define_mnemonic(CMP) {
    if (line->operand_num != 2) {
        throw_error("Only 2 operands are allowed");
    }

    operand_t* op1 = &line->operands[0];
    operand_t* op2 = &line->operands[1];

    if (op1->type == REGISTER_OPERAND && op1->indirect == false && op2->type == REGISTER_OPERAND && op2->indirect == false) {
        append_opcode(0x30);
        append_opcode(*((cpu_register_t*) op1->value));
        append_opcode(*((cpu_register_t*) op2->value));
    } else if (op1->type == REGISTER_OPERAND && op1->indirect == false && op2->type == IMMEDIATE_OPERAND) {
        append_opcode(0x31);
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
    } else {
        throw_error("Invalid operands, CMP instruction was not understood");
    }
}
