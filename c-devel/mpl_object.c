#include "config.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ast_node.h"

static struct mpl_object *mpl_object_eval(
    struct ast_node *this_node,
    size_t variable_count,
    struct mpl_variable *variable_scope,
    size_t function_count,
    struct mpl_function *function_scope
) {
    MPL_DEBUG(fprintf(stderr, "DEBUG: Calling mpl_object->eval() on mpl_object @ 0x%p.\n", (void *)this_node));
    struct mpl_object *result = malloc(sizeof(struct mpl_object));
    memcpy(result, this_node, sizeof(struct mpl_object));
    MPL_DEBUG(fprintf(stderr, "DEBUG:\t\tmpl_object->eval() constructed a new mpl_object @ 0x%p.\n", (void *)result));
    return result;
}

static void mpl_object_destroy_children(struct ast_node *this_node) {
    MPL_DEBUG(fprintf(stderr, "DEBUG: Calling mpl_object->destroy_children() on mpl_object @ 0x%p.\n", (void *)this_node));
    struct mpl_object *this_object = (struct mpl_object *)this_node;
    if (this_object->type == STRING) {
        if (this_object->value.string_value != NULL) {
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
                "DEBUG: Constructing mpl_object with value \"%s\" @ 0x%p.\n", value, (void *)dest
            );
        } else {
            fprintf(
                stderr,
                "DEBUG: Constructing mpl_object with value %s @ 0x%p.\n", value, (void *)dest
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
