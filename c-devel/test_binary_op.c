#include <stdlib.h>
#include <stdio.h>
#include "ast_node.h"

int main(void) {
    struct token *op = malloc(sizeof(struct token));
    construct_token(op, PLUS, "+");
    struct mpl_variable *variable_scope = NULL;
    struct mpl_function *function_scope = NULL;
    struct mpl_object *left = NULL;
    struct mpl_object *right = NULL;
    struct binary_op *operation = construct_binary_op(
        (struct ast_node *)left,
        op,
        (struct ast_node *)right
    );

    if (operation != NULL) {
        struct mpl_object *result = operation->eval(
            (struct ast_node *)operation,
            0, variable_scope,
            0, function_scope
        );

        printf("Obtained ");
        if (result != NULL) {
            switch (result->type) {
                case STRING:
                    printf("%s\n", result->value.string_value);
                    break;
                case FLOAT:
                    printf("%Lf\n", result->value.float_value);
                    break;
                case INT:
                    printf("%lld\n", result->value.int_value);
                    break;
            }
            result->destroy_children((struct ast_node *)result);
            free(result);

        } else {
            printf("NULL returned from operation->eval()\n");
        }

    } else {
        printf("Obtained a NULL object.\n");
    }

    return 0;
}
