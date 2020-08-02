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

struct mpl_object {
    enum mpl_type type;
    union {
        long double float_value;
        long long int_value;
        char *string_value;
    } value;
};

struct variable {
    char *identifier;
    struct mpl_object *value;
};

struct function {
    char *identifier;
    enum mpl_type return_type;
    struct ast_node *body;
};

struct ast_node {
    struct mpl_object *(*eval)(
        struct ast_node *,
        size_t,
        struct variable *,
        size_t,
        struct function *
    );
};

#endif // AST_NODE_H
