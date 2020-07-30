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
    /* stupid-simple MPL program */
    char *program_code
        = "funct foo() {\n  int x = 60 % 3;\n  print \"Hello!\\n\";\n}\n\n# here's a comment\nstart {\n  foo();\n} end\n";

    struct lexer lex = {
        .text = program_code,
        .text_size = strlen(program_code),
        .pos = 0,
        .current_char = program_code[0]
    };

    printf("-=-=-=-=-=-=-=-=-=-=-=-=-\n");
    printf("Program to tokenize:\n%s\n", lex.text);

    /* We slurp up tokens from the code and
       spit them out as soon as we've got them. */
    struct token *tok;
    printf("-=-=-=-=-=-=-=-=-=-=-=-=-\n");
    printf("Got these tokens:\n");
    do {
        tok = get_next_token(&lex);
        if (tok == NULL) {
            printf("<NULL token returned>\n");
        } else {
            printf("%s\n", tok->value);
        }
        free_token(tok);
    } while (tok->type != END_OF_FILE && tok != NULL);
    printf("-=-=-=-=-=-=-=-=-=-=-=-=-\n");

    return 0;
}
