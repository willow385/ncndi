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

struct variable {
    char *identifier;
    enum mpl_type type;
    union {
        long double float_value;
        long long int_value;
        char *string_value;
    } value;
};

struct function {
    char *identifier;
    enum mpl_type return_type;
    struct ast_node *body;
};

struct ast_node {
    // TODO implement
};

#endif // AST_NODE_H
