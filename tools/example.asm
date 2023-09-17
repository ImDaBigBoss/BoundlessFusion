; Text section

init:
    mov r1, 0x12
    mov r2, 0x34
    mov r3, 0x56
    mov r4, 0x78

    ret

; Data section

magical_string:
    db "This is a test string", 0x20, 0x0
