#include <mnemonics.h>

define_mnemonic(SYSCALL) {
    if (line->operand_num != 1) {
        throw_error("The SYSCALL instruction takes no operands!");
    }

    operand_t* op = &line->operands[0];

    if (op->type == IMMEDIATE_OPERAND) {
        append_opcode(0x32);

        union register_value_t {
            uint64_t value;
            uint8_t bytes[8];
        };
        union register_value_t reg_val;
        reg_val.value = *((uint64_t*) op->value);

        for (int i = 0; i < 8; i++) {
            append_opcode(reg_val.bytes[i]);
        }
    } else {
        throw_error("Invalid operands, SYSCALL instruction was not understood");
    }
}
