#include "config.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ast_node.h"


static void mpl_object_destroy_children(struct ast_node *this_node);


static struct mpl_object *mpl_object_eval(
    struct ast_node *this_node,
    size_t *variable_count,
    struct key_object_pair *variable_scope,
    size_t *function_count,
    struct key_program_block_pair *function_scope
) {
    MPL_DEBUG(fprintf(stderr, "DEBUG: Calling mpl_object->eval() on mpl_object @ %p.\n", (void *)this_node));
    struct mpl_object *this_object = (struct mpl_object *)this_node;
    struct mpl_object *result = malloc(sizeof(struct mpl_object));
    result->type = this_object->type;
    result->eval = &mpl_object_eval;
    result->destroy_children = &mpl_object_destroy_children;
    if (this_object->type == STRING) {

        /*
            We can't just assign result->value to the value of this_object->value in this case.
            If we do, then the object we return will have a reference to memory owned by another
            object, and when both objects try to free that memory, we'll get a double-free bug.

            How did I figure this out, you ask? Simple: I compiled with `-fsanitize=address` and
            stepped through the program with `gdb -tui` because I was having double-free bugs.
        */

        result->value.string_value = malloc(1 + strlen(this_object->value.string_value));
        strcpy(result->value.string_value, this_object->value.string_value);
    } else {
        result->value = this_object->value;
    }

    MPL_DEBUG(fprintf(stderr, "DEBUG:\t\tmpl_object->eval() constructed a new mpl_object @ %p.\n", (void *)result));
    return result;

}


static void mpl_object_destroy_children(struct ast_node *this_node) {
    MPL_DEBUG(fprintf(stderr, "DEBUG: Calling mpl_object->destroy_children() on mpl_object @ %p.\n", (void *)this_node));
    struct mpl_object *this_object = (struct mpl_object *)this_node;
    if (this_object->type == STRING) {
        if (this_object->value.string_value != NULL) {
            MPL_DEBUG(fprintf(stderr,
                "DEBUG: The value of the string about to be freed is \"%s\".\n", this_object->value.string_value));
            free(this_object->value.string_value);
            this_object->value.string_value = NULL;
        }
    }
}


void construct_mpl_object(
    struct mpl_object *dest,
    enum mpl_type type,
    const char *value
) {
    MPL_DEBUG(
        if (type == STRING) {
            fprintf(
                stderr,
                "DEBUG: Constructing mpl_object with value \"%s\" @ %p.\n", value, (void *)dest
            );
        } else {
            fprintf(
                stderr,
                "DEBUG: Constructing mpl_object with value %s @ %p.\n", value, (void *)dest
            );
        }
    );

    dest->eval = &mpl_object_eval;
    dest->destroy_children = &mpl_object_destroy_children;
    dest->type = type;
    switch (type) {
        case STRING:
            dest->value.string_value = malloc(1 + strlen(value));
            strcpy(dest->value.string_value, value);
            break;
        case INT:
            sscanf(value, "%lld", &dest->value.int_value);
            break;
        case FLOAT:
            sscanf(value, "%lf", &dest->value.float_value);
            break;
    }
}
