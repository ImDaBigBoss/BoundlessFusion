#include <mnemonics.h>

define_mnemonic(JLE) {
    if (line->operand_num != 1) {
        throw_error("The JLE instruction only takes one operand!");
    }

    operand_t* op = &line->operands[0];
    
    if (op->type == REGISTER_OPERAND && op->indirect == false) {
        append_opcode(0x28);
        append_opcode(0x02); //register mode

        append_opcode(*((cpu_register_t*) op->value));
    } else if (op->type == IMMEDIATE_OPERAND) {
        append_opcode(0x28);
        append_opcode(0x01); //immediate mode

        union register_value_t {
            uint64_t value;
            uint8_t bytes[8];
        };
        union register_value_t reg_val;
        reg_val.value = *((uint64_t*) op->value);

        for (int i = 0; i < 8; i++) {
            append_opcode(reg_val.bytes[i]);
        }
    } else if (op->type == LABEL_OPERAND) {
        append_opcode(0x28);
        append_opcode(0x01); //immediate mode

        char* label_name = (char*) op->value;
        append_label_reference(label_name);
    } else {
        throw_error("Invalid operands, JLE instruction was not understood");
    }
}
