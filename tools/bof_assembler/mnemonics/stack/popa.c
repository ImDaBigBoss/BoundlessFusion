#include <mnemonics.h>

define_mnemonic(POPA) {
    if (line->operand_num != 0) {
        throw_error("The POPA instruction takes no operands!");
    }

    append_opcode(0x45);
}
