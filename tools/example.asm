; Text section

init:
    mov r1, 0x12
    mov r2, 0x34
    mov r3, 0x56
    mov r4, 0x78
    mov r5, 0x9a
    mov r6, 0xbc
    mov r7, 0xde
    mov r8, 0xf0

    ret

; Data section

magical_string:
    db "This is a test string", 0x20, 0x0
