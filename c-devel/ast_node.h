#include <stdlib.h>
#include "token.h"

#ifndef AST_NODE_H
#define AST_NODE_H

/* The types supported by the MPL language. */
enum mpl_type {
    FLOAT,
    INT,
    STRING,
    VOID
};

struct ast_node;
struct mpl_variable;
struct mpl_function;

#define EVAL_METHOD struct mpl_object *(*eval)(\
    struct ast_node *,\
    size_t,\
    struct mpl_variable *,\
    size_t,\
    struct mpl_function *\
);

#define DESTROY_CHILDREN_METHOD void (*destroy_children)(struct ast_node *);

/*
    A program is represented with a struct mpl_program_block containing a
    pointer to an array of children. The children are all heap-allocated,
    so when it's cleanup time, we call the destroy_children() method on
    it before freeing it. The cool thing is that every struct which
    "inherits" from ast_node (meaning it has an eval method and
    destroy_children method at the same memory offsets) has its own
    destroy_children method, which gets called on its own children before
    it too gets freed. In the same way the program evaluates itself by
    recursively calling its children's implementations of eval(), it
    deallocates itself by recursively calling its children's
    implementations of destroy_children().
*/
struct ast_node {
    EVAL_METHOD;
    DESTROY_CHILDREN_METHOD;
};

struct mpl_program_block {
    EVAL_METHOD;
    DESTROY_CHILDREN_METHOD;
    size_t child_count;
    struct ast_node **children;
};

struct mpl_object {
    EVAL_METHOD;
    DESTROY_CHILDREN_METHOD;
    enum mpl_type type;
    union {
        long double float_value;
        long long int_value;
        char *string_value;
    } value;
};

struct mpl_variable {
    EVAL_METHOD;
    DESTROY_CHILDREN_METHOD;
    char *identifier;
    struct mpl_object *value;
};

struct mpl_function {
    EVAL_METHOD;
    DESTROY_CHILDREN_METHOD;
    char *identifier;
    enum mpl_type return_type;
    struct ast_node *body;
};

struct binary_op {
    EVAL_METHOD;
    DESTROY_CHILDREN_METHOD;
    struct ast_node *left;
    struct token *op;
    struct ast_node *right;
};

struct mpl_object *construct_mpl_object(
    enum mpl_type type,
    const char *value
);

struct binary_op *construct_binary_op(
    struct ast_node *left,
    struct token *op,
    struct ast_node *right
);

#endif // AST_NODE_H
