#include <mnemonics.h>

define_mnemonic(RET) {
    if (line->operand_num != 0) {
        throw_error("The RET instruction takes no operands!");
    }

    append_opcode(0x21);
}
