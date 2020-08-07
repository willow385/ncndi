#include "config.h"
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
    MPL_DEBUG(fprintf(stderr, "DEBUG: Calling binary_op->eval() on binary_op @ %p.\n", (void *)this_node));

    /* Cast this_node to the appropriate type to find this_node's children, since
       we know that this_node is actually a binary_op. */
    struct binary_op *this_bin_op = (struct binary_op *)this_node;

    if (this_bin_op->left == NULL
        || this_bin_op->right == NULL
        || this_bin_op->op == NULL
    ) {
        MPL_DEBUG(fprintf(stderr, "DEBUG:\t\tNULL value returned from binary_op->eval().\n"));
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

    if (left == NULL || right == NULL) return NULL;

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

    left->destroy_children((struct ast_node *)left);
    right->destroy_children((struct ast_node *)right);
    MPL_DEBUG(fprintf(stderr, "DEBUG: Freeing memory at %p in binary_op->eval().\n", (void *)left));
    free(left);
    MPL_DEBUG(fprintf(stderr, "DEBUG: Freeing memory at %p in binary_op->eval().\n", (void *)right));
    free(right);

    /* Aaaand we're done! The caller is responsible for
       checking if we returned NULL at this point. */
    return result;
}

static void binary_op_destroy_children(struct ast_node *this_node) {
    MPL_DEBUG(fprintf(
        stderr, "DEBUG: Calling destroy_children() methods on children of binary_op @ %p.\n", (void *)this_node));

    struct binary_op *this_bin_op = (struct binary_op *)this_node;

    if (this_bin_op->left != NULL) {
        this_bin_op->left->destroy_children(this_bin_op->left);
        MPL_DEBUG(fprintf(stderr, "DEBUG: Freeing memory at %p in binary_op->destroy_children().\n",
            (void *)this_bin_op->left));
        free(this_bin_op->left);
    }

    if (this_bin_op->right != NULL) {
        this_bin_op->right->destroy_children(this_bin_op->right);
        MPL_DEBUG(fprintf(stderr, "DEBUG: Freeing memory at %p in binary_op->destroy_children().\n",
            (void *)this_bin_op->right));
        free(this_bin_op->right);
    }

    if (this_bin_op->op != NULL) {
        MPL_DEBUG(fprintf(stderr, "DEBUG: Calling free_token() on %p in binary_op->destroy_children().\n",
            (void *)this_bin_op->op));
        free_token(this_bin_op->op);
    }
}

void construct_binary_op(
    struct binary_op *dest,
    struct ast_node **left,
    struct token *op,
    struct ast_node **right
) {
    MPL_DEBUG(fprintf(stderr, "DEBUG: Constructing binary_op @ %p.\n", (void *)dest));
    dest->left = *left;
    dest->op = op;
    dest->right = *right;
    dest->eval = &binary_op_eval;
    dest->destroy_children = &binary_op_destroy_children;

    /* Because binary_op dest now owns the memory referred to by
       left and right, we set those pointers to NULL, ensuring that
       there's no other references to that memory. */
    *left  = NULL;
    *right = NULL;
}

static struct mpl_object *mpl_object_to_string(struct mpl_object *object) {
    MPL_DEBUG(fprintf(stderr,
        "DEBUG: Converting mpl_object @ %p to mpl_object of type STRING.\n", (void *)object));

    if (object == NULL) {
        MPL_DEBUG(fprintf(stderr, "DEBUG:\t\tObject was NULL; returning NULL.\n"));
        return NULL;
    }

    struct mpl_object *result = malloc(sizeof(struct mpl_object));
    char *result_value;
    switch (object->type) {
        case STRING:
            construct_mpl_object(result, STRING, object->value.string_value);
            break;
        case FLOAT:
            result_value = malloc(1 + snprintf(NULL, 0, "%lf", object->value.float_value));
            sprintf(result_value, "%lf", object->value.float_value);
            construct_mpl_object(result, STRING, result_value);
            free(result_value);
            break;
        case INT:
            result_value = malloc(1 + snprintf(NULL, 0, "%lld", object->value.int_value));
            sprintf(result_value, "%lld", object->value.int_value);
            construct_mpl_object(result, STRING, result_value);
            free(result_value);
            break;
    }

    MPL_DEBUG(fprintf(stderr, "DEBUG:\t\tReturning new mpl_object @ %p.\n", (void *)result));
    return result;
}

static struct mpl_object *multiply_mpl_objects(struct mpl_object *left, struct mpl_object *right) {
    MPL_DEBUG(fprintf(stderr, "DEBUG: multiply_mpl_objects() called.\n"));
    struct mpl_object *result;
    if (left->type == STRING || right->type == STRING) {
        fprintf(stderr, "Error: Operator '*' does not support operands of type string\n");
        result = NULL;
    } else if (left->type == INT && right->type == INT) {
        result = malloc(sizeof(struct mpl_object));
        long long result_value = left->value.int_value * right->value.int_value;
        int bufsize = 1 + snprintf(NULL, 0, "%lld", result_value);
        char *result_value_str = malloc(bufsize);
        sprintf(result_value_str, "%lld", result_value);
        construct_mpl_object(result, INT, result_value_str);
        free(result_value_str);
    } else if (left->type == FLOAT && right->type == INT) {
        result = malloc(sizeof(struct mpl_object));
        double result_value = left->value.float_value * right->value.int_value;
        int bufsize = 1 + snprintf(NULL, 0, "%lf", result_value);
        char *result_value_str = malloc(bufsize);
        sprintf(result_value_str, "%lf", result_value);
        construct_mpl_object(result, FLOAT, result_value_str);
        free(result_value_str);
    } else if (left->type == INT && right->type == FLOAT) {
        result = malloc(sizeof(struct mpl_object));
        double result_value = left->value.int_value * right->value.float_value;
        int bufsize = 1 + snprintf(NULL, 0, "%lf", result_value);
        char *result_value_str = malloc(bufsize);
        sprintf(result_value_str, "%lf", result_value);
        construct_mpl_object(result, FLOAT, result_value_str);
        free(result_value_str);
    } else { /* left->type == FLOAT && right->type == FLOAT */
        result = malloc(sizeof(struct mpl_object));
        double result_value = left->value.float_value * right->value.float_value;
        int bufsize = 1 + snprintf(NULL, 0, "%lf", result_value);
        char *result_value_str = malloc(bufsize);
        sprintf(result_value_str, "%lf", result_value);
        construct_mpl_object(result, FLOAT, result_value_str);
        free(result_value_str);
    }

    return result;
}

static struct mpl_object *add_mpl_objects(struct mpl_object *left, struct mpl_object *right) {
    MPL_DEBUG(fprintf(stderr, "DEBUG: add_mpl_objects() called.\n"));

    struct mpl_object *result;
    if (left->type == STRING || right->type == STRING) {
        // '+' operator overloaded for string concatenation
        struct mpl_object *left_str  = mpl_object_to_string(left);
        struct mpl_object *right_str = mpl_object_to_string(right);
        int result_len = 1 + snprintf(NULL, 0, "%s%s", left_str->value.string_value, right_str->value.string_value);
        char *result_string = malloc(result_len);
        sprintf(result_string, "%s%s", left_str->value.string_value, right_str->value.string_value);
        result = malloc(sizeof(struct mpl_object));
        construct_mpl_object(result, STRING, result_string);
        free(result_string);
        left_str->destroy_children((struct ast_node *)left_str);
        right_str->destroy_children((struct ast_node *)right_str);
        free(left_str); free(right_str);
    } else if (left->type == INT && right->type == INT) {
        // TODO
        result = NULL;
    } else if (left->type == INT && right->type == INT) {
        // TODO
        result = NULL;
    } else if (left->type == INT && right->type == INT) {
        // TODO
        result = NULL;
    } else { /* left->type == FLOAT && right->type == FLOAT */
        // TODO
        result = NULL;
    }

    return result;
}

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
