#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "ast_node.h"

static struct mpl_object *multiply_mpl_objects(struct mpl_object *left, struct mpl_object *right);
static struct mpl_object *add_mpl_objects(struct mpl_object *left, struct mpl_object *right);
static struct mpl_object *divide_mpl_objects(struct mpl_object *left, struct mpl_object *right);
static struct mpl_object *subtract_mpl_objects(struct mpl_object *left, struct mpl_object *right);
static struct mpl_object *modulo_mpl_objects(struct mpl_object *left, struct mpl_object *right);
static struct mpl_object *equal_mpl_objects(struct mpl_object *left, struct mpl_object *right);
static struct mpl_object *greater_mpl_objects(struct mpl_object *left, struct mpl_object *right);
static struct mpl_object *less_mpl_objects(struct mpl_object *left, struct mpl_object *right);
static struct mpl_object *not_eq_mpl_objects(struct mpl_object *left, struct mpl_object *right);
static struct mpl_object *greater_eq_mpl_objects(struct mpl_object *left, struct mpl_object *right);
static struct mpl_object *less_eq_mpl_objects(struct mpl_object *left, struct mpl_object *right);
static struct mpl_object *and_mpl_objects(struct mpl_object *left, struct mpl_object *right);
static struct mpl_object *or_mpl_objects(struct mpl_object *left, struct mpl_object *right);
static struct mpl_object *xor_mpl_objects(struct mpl_object *left, struct mpl_object *right);

static struct mpl_object *binary_op_eval(
    struct ast_node *this_node,
    size_t variable_count,
    struct mpl_variable *variable_scope,
    size_t function_count,
    struct mpl_function *function_scope
) {
    /* Cast this_node to the appropriate type to find this_node's children, since
       we know that this_node is actually a binary_op. */
    struct binary_op *this_bin_op = (struct binary_op *)this_node;

    if (this_bin_op->left == NULL
        || this_bin_op->right == NULL
        || this_bin_op->op == NULL
    ) {
        return NULL;
    }

    /* Evaluate the left operand. */
    struct mpl_object *left = this_bin_op->left->eval(
        this_bin_op->left,
        variable_count,
        variable_scope,
        function_count,
        function_scope
    );

    /* Evaluate the right operand. */
    struct mpl_object *right = this_bin_op->right->eval(
        this_bin_op->right,
        variable_count,
        variable_scope,
        function_count,
        function_scope
    );

    /* Pick the operation to perform. */
    struct mpl_object *result;
    switch (this_bin_op->op->type) {
        case MULT:
            result = multiply_mpl_objects(left, right); break;
        case PLUS:
            result = add_mpl_objects(left, right); break;
        case DIVIDE:
            result = divide_mpl_objects(left, right); break;
        case SUBTRACT:
            result = subtract_mpl_objects(left, right); break;
        case EQUALS:
            result = equal_mpl_objects(left, right); break;
        case GREATER_THAN:
            result = greater_mpl_objects(left, right); break;
        case LESS_THAN:
            result = less_mpl_objects(left, right); break;
        case NOT_EQ:
            result = not_eq_mpl_objects(left, right); break;
        case GREATER_EQ:
            result = greater_eq_mpl_objects(left, right); break;
        case LESS_EQ:
            result = less_eq_mpl_objects(left, right); break;
        case AND:
            result = and_mpl_objects(left, right); break;
        case OR:
            result = or_mpl_objects(left, right); break;
        case XOR:
            result = xor_mpl_objects(left, right); break;
        case MODULUS:
            result = modulo_mpl_objects(left, right); break;
        default:
            result = NULL;
            fprintf(stderr, "Error: Unexpected operator \"%s\" found\n", this_bin_op->op->value);
            break;
    }

    /* Do some cleanup. */
    left->destroy_children((struct ast_node *)left);
    right->destroy_children((struct ast_node *)right);
    free(left);
    free(right);

    /* Aaaand we're done! The caller is responsible for
       checking if we returned NULL at this point. */
    return result;
}

static void binary_op_destroy_children(struct ast_node *this_node) {
    struct binary_op *this_bin_op = (struct binary_op *)this_node;
    this_bin_op->left->destroy_children(this_bin_op->left);
    this_bin_op->right->destroy_children(this_bin_op->right);
    free(this_bin_op->left);
    free_token(this_bin_op->op);
    free(this_bin_op->right);
}

struct binary_op *construct_binary_op(
    struct ast_node *left,
    struct token *op,
    struct ast_node *right
) {
    struct binary_op *result = malloc(sizeof(struct binary_op));
    result->left = left;
    result->op = op;
    result->right = right;
    result->eval = &binary_op_eval;
    result->destroy_children = &binary_op_destroy_children;
    return result;
}

// TODO implement these too
static struct mpl_object *multiply_mpl_objects(struct mpl_object *left, struct mpl_object *right) { return NULL; }
static struct mpl_object *add_mpl_objects(struct mpl_object *left, struct mpl_object *right) { return NULL; }
static struct mpl_object *divide_mpl_objects(struct mpl_object *left, struct mpl_object *right) { return NULL; }
static struct mpl_object *subtract_mpl_objects(struct mpl_object *left, struct mpl_object *right) { return NULL; }
static struct mpl_object *modulo_mpl_objects(struct mpl_object *left, struct mpl_object *right) { return NULL; }
static struct mpl_object *equal_mpl_objects(struct mpl_object *left, struct mpl_object *right) { return NULL; }
static struct mpl_object *greater_mpl_objects(struct mpl_object *left, struct mpl_object *right) { return NULL; }
static struct mpl_object *less_mpl_objects(struct mpl_object *left, struct mpl_object *right) { return NULL; }
static struct mpl_object *not_eq_mpl_objects(struct mpl_object *left, struct mpl_object *right) { return NULL; }
static struct mpl_object *greater_eq_mpl_objects(struct mpl_object *left, struct mpl_object *right) { return NULL; }
static struct mpl_object *less_eq_mpl_objects(struct mpl_object *left, struct mpl_object *right) { return NULL; }
static struct mpl_object *and_mpl_objects(struct mpl_object *left, struct mpl_object *right) { return NULL; }
static struct mpl_object *or_mpl_objects(struct mpl_object *left, struct mpl_object *right) { return NULL; }
static struct mpl_object *xor_mpl_objects(struct mpl_object *left, struct mpl_object *right) { return NULL; }
