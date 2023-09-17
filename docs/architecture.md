# System architecture

Some definitions of concepts used in the documentation:
 - Memory block: A 4kb block of memory (4194304 bytes)

## Registers

There are currently 10 usable registers, each 64 bits wide + 1 flag register:
 - `r1` - `r8`: General purpose registers
 - `sp`: Stack pointer
 - `pc`: Program counter (or instruction pointer)

The flags register is not accessible directly, but is modified by some instructions.

## Memory

The memory is a 64-bit address space. The memory is byte-addressable, meaning that each address points to a single byte.

Please see the syscalls section for more information on how to allocate memory, get the size of the memory, etc.

## Stack

This is just a block of memory that is reserved for this purpose. Much like x86, the stack pointer points to the top of the stack. The stack grows downwards.

## Syscalls

TODO!
