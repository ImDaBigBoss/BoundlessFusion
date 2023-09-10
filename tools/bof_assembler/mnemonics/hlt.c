#include <mnemonics.h>

define_mnemonic(HLT) {
    if (line->operand_num != 0) {
        throw_error("The HLT instruction does not take any operands");
    }

    append_opcode(0xFF);
}
