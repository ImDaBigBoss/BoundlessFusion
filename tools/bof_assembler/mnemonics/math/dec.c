#include <mnemonics.h>

define_mnemonic(DEC) {
    if (line->operand_num != 1) {
        throw_error("Only 1 operand allowed");
    }

    operand_t* op = &line->operands[0];

    if (op->type == REGISTER_OPERAND && op->indirect == false) {
        append_opcode(0x07);
        append_opcode(*((cpu_register_t*) op->value));
    } else {
        throw_error("Invalid operands, DEC instruction was not understood");
    }
}
