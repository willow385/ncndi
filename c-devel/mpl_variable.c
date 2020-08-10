#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include "ast_node.h"


static struct mpl_object *mpl_variable_eval(
    struct ast_node *this_node,
    size_t variable_count,
    struct key_object_pair *variable_scope,
    size_t function_count,
    struct key_program_block_pair *function_scope
) {
    MPL_DEBUG(fprintf(stderr, "DEBUG: Calling mpl_variable->eval() on mpl_variable @ %p.\n", (void *)this_node));
    // TODO implement
    return NULL;
}


static void mpl_variable_destroy_children(struct ast_node *this_node) {
    MPL_DEBUG(fprintf(
        stderr, "DEBUG: Calling destroy_children() methods on children of mpl_variable @ %p.\n", (void *)this_node));
    // TODO implement
}


void construct_mpl_variable(
    struct mpl_variable *dest,
    const char *identifier
) {
    MPL_DEBUG(fprintf(stderr, "DEBUG: Constructing mpl_variable @ %p.\n", (void *)dest));
    // TODO implement
}
