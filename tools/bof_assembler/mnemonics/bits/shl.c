#include <mnemonics.h>

define_mnemonic(SHL) {
    if (line->operand_num != 2) {
        throw_error("Only 2 operands are allowed");
    }

    operand_t* op1 = &line->operands[0];
    operand_t* op2 = &line->operands[1];

    if (op1->type == REGISTER_OPERAND && op1->indirect == false && op2->type == IMMEDIATE_OPERAND && op2->indirect == false) {
        append_opcode(0x0A);
        append_opcode(*((cpu_register_t*) op1->value));
        
        uint64_t value = *((uint64_t*) op2->value);
        if (value > 255) {
            throw_error("Invalid operands, SHL instruction was not understood");
        }
        append_opcode((uint8_t) value);
    } else {
        throw_error("Invalid operands, SHL instruction was not understood");
    }
}
