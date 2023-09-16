; Text section

main:
    mov r1, 4 ; 10 01 04 00 00 00 00 00 00 00     r1 = 4
    mov r2, 3 ; 10 02 03 00 00 00 00 00 00 00     r2 = 3
    mov r3, r2 ; 11 03 02                         r3 = r2
    add r3, r1 ; 01 03 01                         r3 = r3 + r1 (7)
    mov r1, 3 ; 10 01 03 00 00 00 00 00 00 00     r1 = 3
    sub r3, r1 ; 02 03 01                         r3 = r3 - r1 (4)
    mul r3, r2 ; 03 03 02                         r3 = r3 * r2 (12)
    mov r1, 2 ; 10 01 02 00 00 00 00 00 00 00     r1 = 2
    div r3, r1 ; 04 03 01                         r3 = r3 / r1 (6)
    mov r1, 4 ; 10 01 04 00 00 00 00 00 00 00     r1 = 4
    mod r3, r1 ; 05 03 01                         r3 = r3 % r1 (2)

    call $cool_function ; 20 01 [ADDR]

    push r1 ; 40 01

    mov r1, 10 ; 10 01 0A 00 00 00 00 00 00 00    r1 = 10
    mov r2, 20 ; 10 02 14 00 00 00 00 00 00 00    r2 = 20
    inc r1 ; 06 01                                r1 = r1 + 1 (11)
    dec r2 ; 07 02                                r2 = r2 - 1 (19)

    mov r1, 1 ; 10 01 01 00 00 00 00 00 00 00     r1 = 1
    shl r1, 3 ; 0A 01 03                          r1 = r1 << 3 (8)
    shr r1, 1 ; 0B 01 01                          r1 = r1 >> 1 (4)

    mov r2, 0x0f ; 10 02 0f 00 00 00 00 00 00 00  r2 = 0x0f (15)
    and r1, r2 ; 08 01 02                         r1 = r1 & r2 (4)
    or r1, r2 ; 09 01 02                          r1 = r1 | r2 (15)

    syscall 1 ; 32 01 00 00 00 00 00 00 00        call syscall 1

    pop r1 ; 41 01

    ; TODO test jump instructions
    cmp r1, 4 ; 31 01 01 00 00 00 00 00 00 00
    cmp r1, r2 ; 30 01 02

    nop ; 00
    hlt ; FF

cool_function:
    pusha ; 44
    pushf ; 42

    mov r1, $magical_num
    mov r2, [r1]
    mov r3, [r1]
    mov r4, [r1]

    mov r2, 0xcafebabe
    mov [r1], r2

    popf ; 43
    popa ; 45
    ret

; Data section

magical_num:
    db 0xdeadbeef
