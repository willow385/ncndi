#include <stdlib.h>
#include "token.h"

#ifndef AST_NODE_H
#define AST_NODE_H

struct ast_node {
    struct token * (*eval)(
        struct ast_node *this_node,
        struct ast_node *variable_scope,
        size_t variable_count,
        struct ast_node *function_scope,
        size_t function_count
    );
};

#endif // AST_NODE_H
