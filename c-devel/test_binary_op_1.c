#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include "ast_node.h"
#include "token.h"

int main(void) {
    struct mpl_object *left = malloc(sizeof(struct mpl_object));
    struct token *op = malloc(sizeof(struct token));
    struct mpl_object *right = malloc(sizeof(struct mpl_object));
    struct binary_op *root = malloc(sizeof(struct binary_op));

    // Change these values to see how MPL handles various operators and operands.
    construct_mpl_object(left, INT, "2920429980");
    construct_token(op, DIVIDE, "/");
    construct_mpl_object(right, INT, "42069");

    construct_binary_op(root, (struct ast_node **)&left, op, (struct ast_node **)&right);

    struct mpl_variable *variable_scope = NULL;
    struct mpl_function *function_scope = NULL;

    struct mpl_object *result = root->eval(
        (struct ast_node *)root,
        0, variable_scope,
        0, function_scope
    );

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

    root->destroy_children((struct ast_node *)root);
    free(root);

    return 0;
}
