/*
    Compile with:
    $ gcc -c lexer.c
    $ gcc -c token.c
    $ gcc text.c lexer.o token.o

    TODO write a makefile
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "token.h"
#include "lexer.h"

int main(int argc, char *argv[]) {
    char *program_code
        = "funct foo() { int x = 60 % 3; print \"Hello!\\n\"; } start { foo(); } end";

    struct lexer lex = {
        .text = program_code,
        .text_size = strlen(program_code),
        .pos = 0,
        .current_char = program_code[0]
    };

    printf("%s\n", lex.text);

    return 0;
}
