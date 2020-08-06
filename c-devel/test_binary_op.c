#include <stdlib.h>
#include <stdio.h>
#include "ast_node.h"

int main(void) {
    /* The operation to perform on the operands of the binary_op. */
    struct token *op = malloc(sizeof(struct token));
    construct_token(op, PLUS, "+");

    struct mpl_variable *variable_scope = NULL;
    struct mpl_function *function_scope = NULL;
    struct mpl_object *left = NULL;
    struct mpl_object *right = NULL;

    /* The operation we want to perform. */
    struct binary_op *operation = malloc(sizeof(struct binary_op));
    construct_binary_op(
        operation,
        (struct ast_node *)left,
        op,
        (struct ast_node *)right
    );

    /* Remember that any pointers inside an object that has a destroy_children()
       method are expected to point to heap-allocated memory, and that such objects
       are responsible for freeing that memory with said destroy_children() methods. */
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

        operation->destroy_children((struct ast_node *)operation);
        free(operation);

    } else {
        printf("Obtained a NULL object.\n");
    }

    return 0;
}
