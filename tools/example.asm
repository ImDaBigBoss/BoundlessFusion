; Text section

init:
    mov r1, $loop
    syscall 1 ; Set the loop address

    mov r4, 10

    ret

loop:
    ret





    mov r1, 1 ; Level: info
    mov r2, 21 ; String length
    mov r3, $magical_string
    syscall 4 ; Print string

    dec r4
    cmp r4, 0
    je $end

    ret

end:
    hlt

; Data section

magical_string:
    db "This is a test string", 0x0
