# Binary format

## Header

Here is the common part of the header for all versions:

| Offset | Size | Description |
|--------|------|-------------|
| 0      | 4    | Magic number|
| 4      | 1    | Version     |
...

## Version 1

This format is extremely simple, it is composed of a header followed by raw opcodes or data.

### Header

| Offset | Size | Description |
|--------|------|-------------|
| 0      | 4    | Magic number |
| 4      | 1    | Version |
| 5      | 4    | Size of the entire file |
| 9      | 8    | Initial entry point (address including header) |

### Opcodes

Definitions:
 - REG ID: A register ID, from 1 to 10 inclusive
 - IMMU?: An immediate unsigned int of the specified size
 - IMMS?: An immediate signed int of the specified size
 - JMP OPS: 0x01 for immediate, 0x02 for register followed by REG ID or IMMU64 (depending on the previous byte)

| Opcode | Mnemonic | Operands       | Description |
|--------|----------|----------------|-------------|
| 0x0    | NOP      |                | Does nothing |
| -      | -        | -              | - |
| 0x1    | ADD      | REG ID, REG ID | Adds two registers, leaving the result in the first |
| 0x2    | SUB      | REG ID, REG ID | Subtracts two registers, leaving the result in the first |
| 0x3    | MUL      | REG ID, REG ID | Multiplies two registers, leaving the result in the first |
| 0x4    | DIV      | REG ID, REG ID | Divides two registers, leaving the result in the first |
| 0x5    | MOD      | REG ID, REG ID | Modulo two registers, leaving the result in the first |
| 0x6    | INC      | REG ID         | Increments a register |
| 0x7    | DEC      | REG ID         | Decrements a register |
| 0x8    | AND      | REG ID, REG ID | Bitwise AND two registers, leaving the result in the first |
| 0x9    | OR       | REG ID, REG ID | Bitwise OR two registers, leaving the result in the first |
| 0xA    | SHL      | REG ID, IMMU8  | Shifts a register left by an immediate uint8 |
| 0xB    | SHR      | REG ID, IMMU8  | Shifts a register right by an immediate uint8 |
| -      | -        | -              | - |
| 0x10   | MOV      | REG ID, IMMU64 | Moves an immediate uint64 into a register |
| 0x11   | MOV      | REG ID, REG ID | Moves a register into another register |
| 0x12   | MOV      | REG ID, REG ID | Moves a register into memory THIS IS GOING TO CHANGE |
| 0x13   | MOV      | REG ID, REG ID | Moves memory into a register THIS IS GOING TO CHANGE |
| -      | -        | -              | - |
| 0x20   | CALL     | JMP OPS        | Calls a function at an address, this can be returned from |
| 0x21   | RET      |                | Returns from a function |
| 0x22   | JMP      | JMP OPS        | Jumps to an address |
| 0x23   | JE       | JMP OPS        | Jumps to an address if equal (checks the flags register) |
| 0x24   | JNE      | JMP OPS        | Jumps to an address if not equal (checks the flags register) |
| 0x25   | JG       | JMP OPS        | Jumps to an address if greater (checks the flags register) |
| 0x26   | JGE      | JMP OPS        | Jumps to an address if greater or equal (checks the flags register) |
| 0x27   | JL       | JMP OPS        | Jumps to an address if less (checks the flags register) |
| 0x28   | JLE      | JMP OPS        | Jumps to an address if less or equal (checks the flags register) |
| -      | -        | -              | - |
| 0x30   | CMP      | REG ID, REG ID | Compares two registers (as signed int64) and sets the flags register |
| 0x31   | CMP      | REG ID, IMMS64 | Compares a register and an immediate int64 and sets the flags register |
| 0x32   | SYSCALL  | IMMU64         | Calls a syscall |
| -      | -        | -              | - |
| 0x40   | PUSH     | REG ID         | Pushes a register onto the stack |
| 0x41   | POP      | REG ID         | Pops a register from the stack |
| 0x42   | PUSHF    |                | Pushes the flags register onto the stack |
| 0x43   | POPF     |                | Pops the flags register from the stack |
| 0x44   | PUSHA    |                | Pushes all general purpose registers onto the stack |
| 0x45   | POPA     |                | Pops all general purpose registers from the stack |
| -      | -        | -              | - |
| 0xFF   | HLT      |                | Halts the CPU |
