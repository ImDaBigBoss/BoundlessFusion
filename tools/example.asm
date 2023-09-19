; Text section

init:
    mov r1, $loop
    syscall 1 ; Set the game loop address

    mov r1, 1
    mov r2, 27
    mov r3, $magical_string
    syscall 4 ; Print the magical string

    mov r4, 4 ; size of a pixel in bytes
    mov r5, 1 ; colour increment

    ret

loop:
    push r3
    syscall 6 ; Get framebuffer info
    mul r2, r3
    pop r3

loop_fill:
    add r3, r5

    mov [r1], r3
    add r1, r4

    dec r2
    cmp r2, 0
    jne $loop_fill

loop_end:

    ret

; Data section

magical_string:
    db "Example program loading...", 0x0
