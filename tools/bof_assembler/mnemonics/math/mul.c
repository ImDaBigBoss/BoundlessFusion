#include <mnemonics.h>

define_mnemonic(MUL) {
    if (line->operand_num != 2) {
        throw_error("Only 2 operands are allowed");
    }

    operand_t* op1 = &line->operands[0];
    operand_t* op2 = &line->operands[1];

    if (op1->type == REGISTER_OPERAND && op1->indirect == false && op2->type == REGISTER_OPERAND && op2->indirect == false) {
        append_opcode(0x03);
        append_opcode(*((cpu_register_t*) op1->value));
        append_opcode(*((cpu_register_t*) op2->value));
    } else {
        throw_error("Invalid operands, MUL instruction was not understood");
    }
}
