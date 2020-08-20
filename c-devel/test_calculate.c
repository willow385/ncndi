#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#include "ast_node.h"
#include "lexer.h"
#include "parser.h"

int main(void) {

    char *math_expression = "3 + ((3 * 15) / 5)";

    printf(
        "Evaluating expression \"%s\". It should result in %d.\n",
        math_expression,
        3 + ((3 * 15) / 5)
    );

    /* Set up objects */
    struct lexer lex = {
        .text = math_expression,
        .text_size = strlen(math_expression),
        .pos = 0,
        .current_char = math_expression[0]
    };
    struct token *current_token = get_next_token(&lex);
    struct parser parse = {
        .lex = &lex,
        .current_token = current_token,
        .just_parsed_program_block = 0
    };

    /* Parse the code into an Abstract Syntax Tree */
    struct ast_node *root = expression(&parse);

    /* Get stuff ready to evaluate the AST */
    size_t variable_count = 0, function_count = 0;
    struct key_program_block_pair *function_scope = NULL;
    struct key_object_pair *variable_scope = NULL;

    /* Evaluate the AST */
    struct mpl_object *result = root->eval(
        root,
        &variable_count, variable_scope,
        &function_count, function_scope
    );

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
    root->destroy_children(root);
    free(root);

    MPL_DEBUG(fprintf(stderr, "DEBUG: Freeing variable_scope and function_scope now.\n"));
    /* don't forget to free variable_scope - this is the caller's responsibility! */
    size_t i;
    for (i = 0; i < variable_count; ++i) {
        variable_scope[i].value->destroy_children((struct ast_node *)variable_scope[i].value);
        free(variable_scope[i].value);
    }

    for (i = 0; i < function_count; ++i) {
        function_scope[i].value->destroy_children((struct ast_node *)function_scope[i].value);
        free(function_scope[i].value);
    }

    free(variable_scope);
    free(function_scope);

    return 0;

}
