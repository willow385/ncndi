#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include "ast_node.h"
#include "token.h"


int main(void) {

    struct mpl_object *result = root->eval(
        (struct ast_node *)root,
        0, variable_scope,
        0, function_scope
    );

    if (result != NULL) {
        switch (result->type) {
            case STRING:
                printf("Obtained \"%s\" from root->eval()\n", result->value.string_value);
                break;
            case FLOAT:
                printf("Obtained %lf from root->eval()\n", result->value.float_value);
                break;
            case INT:
                printf("Obtained %lld from root->eval()\n", result->value.int_value);
                break;
        }

        result->destroy_children((struct ast_node *)result);
        free(result);

    } else {

        printf("Obtained NULL result from root->eval()\n");

    }

    root->destroy_children((struct ast_node *)root);
    free(root);

    return 0;

}
