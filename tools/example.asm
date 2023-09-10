;Our usable registers:
; r1, r2, r3, r4 (general purpose)
; sp (stack pointer), pc (program counter)

main:
    mov r1, 4 ; 10 01 04 00 00 00 00 00     r1 = 4
    mov r2, 3 ; 10 02 03 00 00 00 00 00     r2 = 3
    mov r3, r2 ; 11 03 02                   r3 = r2
    add r3, r1 ; 01 03 01                   r3 = r3 + r1 (7)
    mov r1, 3 ; 10 01 03 00 00 00 00 00     r1 = 3
    sub r3, r1 ; 02 03 01                   r3 = r3 - r1 (4)
    mul r3, r2 ; 03 03 02                   r3 = r3 * r2 (12)
    mov r1, 2 ; 10 01 02 00 00 00 00 00     r1 = 2
    div r3, r1 ; 04 03 01                   r3 = r3 / r1 (6)
    mov r1, 4 ; 10 01 04 00 00 00 00 00     r1 = 4
    mod r3, r1 ; 05 03 01                   r3 = r3 % r1 (2)

    call $cool_function ; 20 01 [ADDR]

    nop ; 00
    hlt ; ff

cool_function:
    mov r1, 0xdeadbeef
    mov r2, 0xdeadbeef
    mov r3, 0xdeadbeef
    mov r4, 0xdeadbeef
    ret

magical_num:
    db 0xdeadbeef

msg:
    db "Hello, World!", 0xa
