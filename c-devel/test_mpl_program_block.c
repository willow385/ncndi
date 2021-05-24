#include "config.h"
#include <stdlib.h>
#include <stdio.h>
#include "ast_node.h"

int main(void) {
    struct key_object_pair *variable_scope = NULL;
    struct key_program_block_pair *function_scope = NULL;
    size_t variable_count = 0;
    size_t function_count = 0;

    struct mpl_program_block *root = malloc(sizeof(
        struct mpl_program_block
    ));
    construct_mpl_program_block(root);

    struct binary_op *statement_0 = malloc(sizeof(
        struct binary_op
    ));

    struct mpl_object **left_0 = malloc(sizeof(struct mpl_object *));
    *left_0 = malloc(sizeof(struct mpl_object));
    construct_mpl_object(*left_0, INT, "2");

    struct token *op_0 = malloc(sizeof(struct token));
    construct_token(op_0, PLUS, "+");

    struct mpl_object **right_0 = malloc(sizeof(struct mpl_object *));
    *right_0 = malloc(sizeof(struct mpl_object));
    construct_mpl_object(*right_0, INT, "2");

    construct_binary_op(
        statement_0,
        (struct ast_node **)left_0,
        op_0,
        (struct ast_node **)right_0
    );

    struct binary_op *statement_1 = malloc(sizeof(
        struct binary_op
    ));

    struct mpl_object **left_1 = malloc(sizeof(struct mpl_object *));
    *left_1 = malloc(sizeof(struct mpl_object));
    construct_mpl_object(*left_1, INT, "2");

    struct token *op_1 = malloc(sizeof(struct token));
    construct_token(op_1, PLUS, "+");

    struct mpl_object **right_1 = malloc(sizeof(struct mpl_object *));
    *right_1 = malloc(sizeof(struct mpl_object));
    construct_mpl_object(*right_1, INT, "3");

    construct_binary_op(
        statement_1,
        (struct ast_node **)left_1,
        op_1,
        (struct ast_node **)right_1
    );

    mpl_program_block_append_child(root, (struct ast_node **)&statement_0);
    mpl_program_block_append_child(root, (struct ast_node **)&statement_1);

    if (root != NULL) {

        struct mpl_object *result = root->eval(
            (struct ast_node *)root,
            &variable_count, variable_scope,
            &function_count, function_scope
        );

        if (result != NULL) {
            // should print 5
            printf("Result: %lld\n", result->value.int_value);
        } else {
            printf("Result was NULL\n");
        }

        root->destroy_children((struct ast_node *)root);
        free(root);

    } else {

        printf("Result was NULL\n");

    }

    return 0;
}
