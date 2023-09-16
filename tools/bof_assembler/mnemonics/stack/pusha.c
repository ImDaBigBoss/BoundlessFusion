#include <mnemonics.h>

define_mnemonic(PUSHA) {
    if (line->operand_num != 0) {
        throw_error("The PUSHA instruction takes no operands!");
    }

    append_opcode(0x44);
}
