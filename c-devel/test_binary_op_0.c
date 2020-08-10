#include "config.h"
#include <stdlib.h>
#include <stdio.h>
#include "ast_node.h"


int main(void) {

    /*
        We're going to build a rudimentary AST that looks like this:

                                binary_op
                                /   |   \
                               /    |    \
                              /     |     \
                             /      |      \
                            /       |       \
                       binary_op    +       " is nice :)"
                       /   |   \
                      /    |    \
                     /     |     \
                    /      |      \
                   /       |       \
         "The number "     +       69

    */

    struct key_object_pair *variable_scope = NULL;
    struct key_program_block_pair *function_scope = NULL;

    /*
        The lines below this comment look ugly because we're directly calling malloc()
        and the constructor functions. Later, when the parser is implemented, we'll
        simply get a pointer to a complete AST by sticking the lexer into a function,
        probably something like

            struct mpl_program_block *root = parser_gen_ast(&lex);

        And all we'll have to do from there is call root->eval() to run the program.
    */
    struct binary_op  *root          = malloc(sizeof(struct binary_op));
    struct binary_op  *level_0_left  = malloc(sizeof(struct binary_op));
    struct token      *level_0_op    = malloc(sizeof(struct token));
    struct mpl_object *level_0_right = malloc(sizeof(struct mpl_object));
    struct mpl_object *level_1_left  = malloc(sizeof(struct mpl_object));
    struct token      *level_1_op    = malloc(sizeof(struct token));
    struct mpl_object *level_1_right = malloc(sizeof(struct mpl_object));

    construct_mpl_object(level_1_left, STRING, "The number ");
    construct_token(level_1_op, PLUS, "+");
    construct_mpl_object(level_1_right, INT, "69");

    construct_binary_op(level_0_left, (struct ast_node **)&level_1_left, level_1_op, (struct ast_node **)&level_1_right);
    construct_token(level_0_op, PLUS, "+");
    construct_mpl_object(level_0_right, STRING, " is nice :)");

    construct_binary_op(root, (struct ast_node **)&level_0_left, level_0_op, (struct ast_node **)&level_0_right);

    /* Remember that any pointers inside an object that has a destroy_children()
       method are expected to point to heap-allocated memory, and that such objects
       are responsible for freeing that memory with said destroy_children() methods.

       See ast_node.h for more details.
    */
    if (root != NULL) {

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

    } else {

        printf("Obtained a NULL object.\n");

    }

    return 0;

}
