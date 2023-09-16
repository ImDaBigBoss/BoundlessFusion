#include <mnemonics.h>

define_mnemonic(PUSHF) {
    if (line->operand_num != 0) {
        throw_error("The PUSHF instruction takes no operands!");
    }

    append_opcode(0x42);
}
