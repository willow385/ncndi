/*
    Compile with:
    $ gcc -c lexer.c
    $ gcc text.c lexer.o

    TODO write a makefile
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "token.h"
#include "lexer.h"

int main(void) {
    char *program_code = "69420   3.14159";

    struct lexer lex = {
        .text = program_code,
        .text_size = strlen(program_code),
        .pos = 0,
        .current_char = program_code[0]
    };

    /* parse_number  will parse a token from the text
       in our struct lexer and return it as a pointer
       to a malloc'd struct token. */
    struct token *first_token = parse_number(&lex);

    printf("Parsed this number: %s\n", first_token->value);
    if (first_token->type == INTEGER) {
        printf("It's an integer!\n");
    } else if (first_token->type == FLOAT) {
        printf("It's a float!\n");
    }

    printf("Skipping whitespace... ");
    skip_whitespace(&lex);
    printf("Whitespace skipped.\n");

    struct token *second_token = parse_number(&lex);

    printf("Parsed this number: %s\n", second_token->value);
    if (second_token->type == INTEGER) {
        printf("It's an integer!\n");
    } else if (second_token->type == FLOAT) {
        printf("It's a float!\n");
    }

    free(first_token->value);
    free(first_token);
    free(second_token->value);
    free(second_token);
    return 0;
}
