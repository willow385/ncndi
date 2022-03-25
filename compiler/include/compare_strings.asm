; Subroutine to check if two null-terminated strings are the same.
; Leaves the result in register A: 1 if they're the same, 0 otherwise.
; Push the return address, then the pointer to the first string, then
; the pointer to the second string, before jumping here.
@__compare_strings__:
    pop     rb              ; char *string_0
    pop     rc              ; char *string_1
@__compare_strings_L0:
    readr   rd  rc          ; fetch a char from string_1
    readr   ra  rb          ; fetch a char from string_0
    ; if (*string_0 == *string_1) goto __compare_strings_L0_nextchar
    jeq     ra  rd  @__compare_strings_L0_nextchar
    ; else return 0
    jmp             @__compare_strings_ret0
@__compare_strings_L0_nextchar:
    ; if we've reached null chars for both strings, then we're done.
    push    rc              ; save the pointer to string_0
    xor     rc  rc          ; zero out register C for comparison
    ; if (*string_0 == '\0') return 1
    jeq     ra  rc  @__compare_strings_ret1
    ; else goto __compare_strings_L0
    pop     rc              ; load the pointer to string_0
    inc     rc              ; string_0++
    inc     rb              ; string_1++
    jmp             @__compare_strings_L0
@__compare_strings_ret0:
    ; we should jump here if the strings are not the same
    xor     ra  ra
    pop     ip
@__compare_strings_ret1:
    ; pop the value we pushed from rc
    pop     rc

    ; we should jump here if the strings are the same
    movl    ra      1
    pop     ip  
