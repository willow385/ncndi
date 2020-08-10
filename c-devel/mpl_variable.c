#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast_node.h"


static struct mpl_object *mpl_variable_eval(
    struct ast_node *this_node,
    size_t *variable_count,
    struct key_object_pair *variable_scope,
    size_t *function_count,
    struct key_program_block_pair *function_scope
) {
    MPL_DEBUG(fprintf(stderr,
        "DEBUG: Calling mpl_variable->eval() on mpl_variable \"%s\" @ %p.\n",
        ((struct mpl_variable *)this_node)->identifier,
        (void *)this_node)
    );

    struct mpl_variable *this_var = (struct mpl_variable *)this_node;

    size_t i;
    for (i = 0; i < *variable_count; ++i) {
        if (!strcmp(this_var->identifier, variable_scope[i].key)) {
            return variable_scope[i].value->eval(
                (struct ast_node *)variable_scope[i].value,
                variable_count, variable_scope,
                function_count, function_scope
            );
        }
    }

    MPL_DEBUG(fprintf(stderr, "DEBUG:\t\tNo such variable found in current scope.\n"));
    return NULL;

}


static void mpl_variable_destroy_children(struct ast_node *this_node) {
    MPL_DEBUG(fprintf(
        stderr, "DEBUG: Calling destroy_children() methods on children of mpl_variable @ %p.\n", (void *)this_node));
    free(((struct mpl_variable *)this_node)->identifier);
}


void construct_mpl_variable(
    struct mpl_variable *dest,
    const char *identifier
) {
    MPL_DEBUG(fprintf(stderr, "DEBUG: Constructing mpl_variable with identifier \"%s\" @ %p.\n",
        identifier, (void *)dest));

    dest->identifier = malloc(1 + strlen(identifier));
    strcpy(dest->identifier, identifier);
    dest->eval = &mpl_variable_eval;
    dest->destroy_children = &mpl_variable_destroy_children;
}
