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
    /* The "program" we're lexing here is just an integer
       followed by a string literal. */
    char *program_code
        = "69420  \"string\\t \\\"literal\\\" \\r\\n%-%%%-% C:\\\\Users\\\\admin \\n\"";

    struct lexer lex = {
        .text = program_code,
        .text_size = strlen(program_code),
        .pos = 0,
        .current_char = program_code[0]
    };

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

    struct token *second_token = parse_string_literal(&lex);

    /* If  parse_string_literal()  works right, the output should
       contain a bunch of backslashes, whitespaces, and newlines. */
    printf("Parsed this string literal: \"%s\"\n", second_token->value);

    free_token(first_token);
    free_token(second_token);
    return 0;
}
