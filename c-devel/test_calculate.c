#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#include "ast_node.h"
#include "lexer.h"
#include "parser.h"

int main(int argc, char *argv[]) {

    char *program_code;
    char buf[1024];
    int repl_mode = 0;
    if (argc < 2) {
        repl_mode = 1;
        program_code = buf;
    } else {
        program_code = argv[1];
    }

    if (!repl_mode) {
        printf("Evaluating expression \"%s\".\n", program_code);
    } else {
        printf("Simple calculator REPL written entirely in C by Dante Falzone\n");
        printf("Supported operators: ( ) * / + - %% == != > < >= <= && || <>\n");
        printf("Use the command \"exit\" to safely exit the REPL\n");
    }

beginning_of_loop:
    do {
        if (repl_mode) {
            printf(">> ");
            fgets(program_code, 1024, stdin);

            if (!strcmp(program_code, "exit\n") || !strcmp(program_code, "exit\r\n"))
                break;

            if (!strcmp(program_code, "\n") || !strcmp(program_code, "\r\n"))
                goto beginning_of_loop;
        }

        /* Set up objects */
        struct lexer lex = {
            .text = program_code,
            .text_size = strlen(program_code),
            .pos = 0,
            .current_char = program_code[0]
        };
        struct parser parse = {
            .lex = &lex,
            .current_token = NULL,
            .just_parsed_program_block = 0
        };

        /* Parse the code into an Abstract Syntax Tree */
        struct ast_node *root = parser_gen_ast(&parse);
        /* root will be NULL if no code was entered */

        /* Get stuff ready to evaluate the AST */
        size_t variable_count = 0, function_count = 0;

        /* Evaluate the AST */
        struct mpl_object *result = NULL;
        if (root != NULL) {
            result = root->eval(
                root,
                &variable_count, NULL,
                &function_count, NULL
            );
        }

        /* Print the result */
        if (result != NULL) {
            switch (result->type) {
                case STRING:
                    printf("Obtained \"%s\" from root->eval()\n", result->value.string_value);
                break;

                case FLOAT:
                    printf("Obtained %lf from root->eval()\n", result->value.float_value);
                break;

                case INT:
                    printf("Obtained %lld from root->eval()\n", result->value.int_value);
                break;
            }
            result->destroy_children((struct ast_node *)result);
            free(result);
        } else {
            printf("Obtained NULL result from root->eval()\n");

        }

        /* Cleanup */
        if (root != NULL) {
            root->destroy_children(root);
            free(root);
        }

    } while (repl_mode);

    return 0;

}
