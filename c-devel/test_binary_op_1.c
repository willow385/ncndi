#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include "ast_node.h"
#include "token.h"


int main(void) {

    size_t variable_count = 0;
    size_t function_count = 0;

    struct key_program_block_pair *function_scope = NULL;

    /*
        We want to have a single variable in the global scope, "test_variable", which
        is an int and equals 69000.
    */
    struct key_object_pair *variable_scope = NULL;
    struct mpl_object *left_value = malloc(sizeof(struct mpl_object));
    construct_mpl_object(left_value, INT, "69000");
    append_key_object_pair(&variable_scope, &variable_count, "test_variable", left_value);


    struct mpl_variable *left = malloc(sizeof(struct mpl_variable));
    construct_mpl_variable(left, "test_variable");


    struct token *op = malloc(sizeof(struct token));
    construct_token(op, PLUS, "+");


    struct mpl_object *right = malloc(sizeof(struct mpl_object));
    construct_mpl_object(right, INT, "420");


    struct binary_op *root = malloc(sizeof(struct binary_op));
    construct_binary_op(root, (struct ast_node **)&left, op, (struct ast_node **)&right);


    struct mpl_object *result = root->eval(
        (struct ast_node *)root,
        &variable_count, variable_scope,
        &function_count, function_scope
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

    MPL_DEBUG(fprintf(stderr, "DEBUG: Freeing variable_scope and function_scope now.\n"));
    /* don't forget to free variable_scope - this is the caller's responsibility! */
    size_t i;
    for (i = 0; i < variable_count; ++i) {
        variable_scope[i].value->destroy_children((struct ast_node *)variable_scope[i].value);
    }

    for (i = 0; i < function_count; ++i) {
        variable_scope[i].value->destroy_children((struct ast_node *)variable_scope[i].value);
    }

    free(variable_scope);
    free(function_scope);

    return 0;

}
