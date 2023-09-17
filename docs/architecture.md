# System architecture

Some definitions of concepts used in the documentation:
 - Memory block: A 4kb block of memory (4194304 bytes)

At any time, the program can be executed using the halt instruction.

## Registers

There are currently 10 usable registers, each 64 bits wide + 1 flag register:
 - `r1` - `r8`: General purpose registers
 - `sp`: Stack pointer
 - `pc`: Program counter (or instruction pointer)

The flags register is not accessible directly, but is modified by some instructions.

## Memory

The memory is a 64-bit address space. The memory is byte-addressable, meaning that each address points to a single byte. The engine starts with 1 memory block, but more can be allocated using syscalls.

## Stack

This is just a block of memory that is reserved for this purpose. Much like x86, the stack pointer points to the top of the stack. The stack grows downwards.

## Syscalls

| Syscall | Description |
|---------|-------------|
| 1       | Define loop address. Set R1 to the address that is run during the game loop. |
| 2       | Get the memory information. R1 is the address at which user memory starts, R2 is the number of memory blocks. |
| 3       | Allocate memory. R1 is the number of memory blocks to allocate. |
| 4       | Debug print. R1 is the debug level (0: raw, 1: info, 2: warning, 3: error), R2 is the length of the string, R3 is the address of the string. |
| 5       | Dump registers. This will dump all registers to the debug log. |
