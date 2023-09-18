; Text section

init:
    mov r1, $loop
    syscall 1 ; Set the loop address

    syscall 6 ; Get framebuffer info

    mul r2, r3
    mov r3, 0
    mov r4, 2
    mov r5, 0x0000000100000001
    mov r6, 0
    ret

loop:
    push r1
    push r2

loop_fill:
    add r3, r5

    mov [r1], r3
    add r1, r4

    sub r2, r4
    cmp r2, 0
    jne $loop_fill

loop_end:
    pop r2
    pop r1
    ret

; Data section

magical_string:
    db "This is a test string", 0x0
