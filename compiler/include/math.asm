; Subroutine to divide two numbers. To call this subroutine,
; first push the return address, then the number you want to
; divide, then the number by which you want to divide it.
; This program will leave the quotient in register A and the
; remainder in register B.
@__divide__:
    pop     rc                  ; denominator
    pop     rd                  ; numerator
    xor     ra  ra              ; quotient
    xor     rb  rb              ; remainder
    jeq     rc  ra  @__divide_by_zero
    jeq     rc  rd  @__divide_ez
    movr    rb  rd
@__divide_loop:
    jgr     rc  rb  @__divide_return
    jeq     rc  rb  @__divide_return
    inc     ra
    sub     rb  rc
    jmp             @__divide_loop
@__divide_return:
    pop     ip
@__divide_ez: ; If divisor equals dividend then our job is easy.
    movl    ra      1
    movl    rb      0
    pop     ip
@__divide_by_zero:
    movl    ra      'D'
    out     ra
    movl    ra      'I'
    out     ra
    movl    ra      'V'
    out     ra
    movl    ra      'I'
    out     ra
    movl    ra      'D'
    out     ra
    movl    ra      'E'
    out     ra
    movl    ra      '\s'
    out     ra
    movl    ra      'B'
    out     ra
    movl    ra      'Y'
    out     ra
    movl    ra      '\s'
    out     ra
    movl    ra      '0'
    out     ra
    movl    ra      '\n'
    out     ra
    halt
