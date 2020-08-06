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
    struct mpl_object *result = malloc(sizeof(struct mpl_object));
    memcpy(result, this_node, sizeof(struct mpl_object));
    return result;
}

static void mpl_object_destroy_children(struct ast_node *this_node) {
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
    dest->eval = &mpl_object_eval;
    dest->destroy_children = &mpl_object_destroy_children;
    dest->type = type;
    switch (type) {
        case STRING:
            dest->value.string_value = malloc(1 + strlen(value));
            strcpy(dest->value.string_value, value);
            break;
        case INT:
            dest->value.int_value = strtoll(value, NULL, 10);
            break;
        case FLOAT:
            dest->value.float_value = strtold(value, NULL);
            break;
    }
}
