#include <mnemonics.h>

define_mnemonic(POPF) {
    if (line->operand_num != 0) {
        throw_error("The POPF instruction takes no operands!");
    }

    append_opcode(0x43);
}
