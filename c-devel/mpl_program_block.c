#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#include "ast_node.h"


static struct mpl_object *mpl_program_block_eval(
    struct ast_node *this_node,
    size_t *variable_count,
    struct key_object_pair *variable_scope,
    size_t *function_count,
    struct key_program_block_pair *function_scope
) {
    MPL_DEBUG(fprintf(stderr, "DEBUG: Calling mpl_program_block->eval() on mpl_program_block @ %p.\n", (void *)this_node));
    struct mpl_program_block *this_block = (struct mpl_program_block *)this_node;

    int i;
    struct mpl_object *result;

    if (this_block->children == NULL) {
        return NULL;
    }

    for (i = 0; i < this_block->child_count; i++) {
        result = this_block->children[i]->eval(
            (struct ast_node *)this_block,
            variable_count, variable_scope,
            function_count, function_scope
        );

        if (i == this_block->child_count - 1) {
            return result;
        } else {
            free(result);
            return NULL;
        }
    }
}


static void mpl_program_block_destroy_children(struct ast_node *this_node) {
    MPL_DEBUG(fprintf(
        stderr, "DEBUG: Calling destroy_children() methods on children of mpl_program_block @ %p.\n", (void *)this_node));
    struct mpl_program_block *this_block = (struct mpl_program_block *)this_node;

    if (this_block->children != NULL) {
        int i;
        for (i = 0; i < this_block->child_count; i++) {
            this_block->children[i]->destroy_children(
                this_block->children[i]
            );
            free(this_block->children[i]);
        }
        free(this_block->children);
    }
}


void construct_mpl_program_block(struct mpl_program_block *dest) {
    dest->node_type = MPL_PROGRAM_BLOCK;
    dest->eval = &mpl_program_block_eval;
    dest->destroy_children = &mpl_program_block_destroy_children;
    dest->child_count = 0;
    dest->children = NULL;
}


void mpl_program_block_append_child(
    struct mpl_program_block *dest,
    struct ast_node **child
) {
    MPL_DEBUG(
        if (child == NULL) {
            fprintf(stderr,
                "DEBUG: Attempting to append NULL to mpl_program_block @ %p.\n", (void *)dest
            );
        } else {
            fprintf(stderr,
                "DEBUG: Attempting to append ast_node @ %p to mpl_program_block @ %p.\n",
                (void *)*child, (void *)dest
            );
        }
    );

    if (child == NULL) return;
    if (*child == NULL) return;

    dest->child_count++;
    dest->children = realloc(dest->children, sizeof(struct ast_node *) * dest->child_count);
    dest->children[dest->child_count - 1] = *child;
    *child = NULL;
}
