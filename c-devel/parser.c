#include "config.h"
#include <stdlib.h>
#include <stdio.h>
#include "token.h"
#include "lexer.h"
#include "ast_node.h"
#include "parser.h"


int eat(struct parser *parse, enum token_type type) {

    MPL_DEBUG(fprintf(stderr,
        "DEBUG: Calling eat() on parser @ %p with current token \"%s\".\n",
        (void *)parse, parse->current_token->value
    ));

    if (parse->current_token->type == type) {
        free_token(parse->current_token);
        parse->current_token = get_next_token(parse->lex);
        MPL_DEBUG(fprintf(stderr, "DEBUG:\t\teat() is returning 0.\n"));
        return 0;
    }

    free_token(parse->current_token);
    MPL_DEBUG(fprintf(stderr, "DEBUG:\t\teat() is returning 1.\n"));
    return 1;
}


// TODO implement
struct mpl_program_block *program(struct parser *parse);


struct ast_node *factor(struct parser *parse) {
    MPL_DEBUG(fprintf(stderr,
        "DEBUG: Calling factor() on parser @ %p with current token \"%s\".\n",
        (void *)parse, parse->current_token->value
    ));

    struct token *tok = malloc(sizeof(struct token));
    construct_token(tok, parse->current_token->type, parse->current_token->value);
    struct ast_node *result;

    switch (tok->type) {
        case INT_L:
            eat(parse, INT_L);
            result = malloc(sizeof(struct mpl_object));
            construct_mpl_object((struct mpl_object *)result, INT, tok->value);
            free_token(tok);
        break;

        case FLOAT_L:
            eat(parse, FLOAT_L);
            result = malloc(sizeof(struct mpl_object));
            construct_mpl_object((struct mpl_object *)result, FLOAT, tok->value);
            free_token(tok);
        break;

        case STRING_L:
            eat(parse, STRING_L);
            result = malloc(sizeof(struct mpl_object));
            construct_mpl_object((struct mpl_object *)result, STRING, tok->value);
            free_token(tok);
        break;

        case OPEN_PAREN:
            eat(parse, OPEN_PAREN);
            result = expression(parse);
            free_token(tok);
            eat(parse, CLOSE_PAREN);
        break;

        case IDENTIFIER:
            // TODO return mpl_variable or mpl function call
        break;

        case IF:
            // TODO return if-statement
        break;

        case NOT:
            // TODO return logical negation
        break;

        case SUBTRACT:
            eat(parse, SUBTRACT);
            free_token(tok);
            result = factor(parse);
            struct mpl_object *borrow_result = (struct mpl_object *)result;
            if (borrow_result->type == INT) {
                borrow_result->value.int_value *= -1;
            } else if (borrow_result->type == FLOAT) {
                borrow_result->value.float_value *= -1;
            } else {
                fprintf(stderr, "Error: Unexpected string \"%s\" encountered\n",
                    borrow_result->value.string_value);

                result->destroy_children(result);
                free(result);
                result = NULL;
            }
        break;

        default:
            fprintf(stderr, "Error: Unexpected token \"%s\" encountered\n", tok->value);
            free_token(tok);
            result = NULL;
        break;
    }

    MPL_DEBUG(fprintf(stderr,
        "DEBUG:\t\tfactor() is returning a new struct ast_node @ %p.\n", (void *)result));

    return result;
}


struct ast_node *term(struct parser *parse) {
    MPL_DEBUG(fprintf(stderr,
        "DEBUG: Calling term() on parser @ %p with current token \"%s\".\n",
        (void *)parse, parse->current_token->value
    ));

    struct ast_node *node = factor(parse);
    struct token *tok;

    while (
        parse->current_token->type == MULT
        || parse->current_token->type == DIVIDE
        || parse->current_token->type == MODULUS
    ) {
        tok = malloc(sizeof(struct token));
        construct_token(tok, parse->current_token->type, parse->current_token->value);
        eat(parse, tok->type);
        struct ast_node *left = node;
        struct ast_node *right = factor(parse);
        node = malloc(sizeof(struct binary_op));
        construct_binary_op((struct binary_op *)node, &left, tok, &right);
    }

    MPL_DEBUG(fprintf(stderr,
        "DEBUG:\t\tterm() is returning a new struct ast_node @ %p.\n", (void *)node));

    return node;
}


struct ast_node *math_expression(struct parser *parse) {
    MPL_DEBUG(fprintf(stderr,
        "DEBUG: Calling math_expression() on parser @ %p with current token \"%s\".\n",
        (void *)parse, parse->current_token->value
    ));

    struct ast_node *node = term(parse);
    struct token *tok;

    while (parse->current_token->type == PLUS || parse->current_token->type == SUBTRACT) {
        tok = malloc(sizeof(struct ast_node));
        construct_token(tok, parse->current_token->type, parse->current_token->value);
        eat(parse, tok->type);
        struct ast_node *left = node;
        struct ast_node *right = term(parse);
        node = malloc(sizeof(struct binary_op));
        construct_binary_op((struct binary_op *)node, &left, tok, &right);
    }

    MPL_DEBUG(fprintf(stderr,
        "DEBUG:\t\tmath_expression() is returning a new struct ast_node @ %p.\n", (void *)node));

    return node;
}


#define IS_LOGICAL_OP(x)  \
    x == AND              \
    || x == OR            \
    || x == XOR

struct ast_node *expression(struct parser *parse) {
    MPL_DEBUG(fprintf(stderr,
        "DEBUG: Calling expression() on parser @ %p with current token \"%s\".\n",
        (void *)parse, parse->current_token->value
    ));

    struct ast_node *node = comparison(parse);
    struct token *tok;

    while (IS_LOGICAL_OP(parse->current_token->type)) {
        tok = malloc(sizeof(struct token));
        construct_token(tok, parse->current_token->type, parse->current_token->value);
        eat(parse, tok->type);
        struct ast_node *left = node;
        node = malloc(sizeof(struct binary_op));
        struct ast_node *right = comparison(parse);
        construct_binary_op((struct binary_op *)node, &left, tok, &right);
    }

    MPL_DEBUG(fprintf(stderr,
        "DEBUG:\t\texpression() is returning a new struct ast_node @ %p.\n", (void *)node));

    return node;
}


#define IS_RELATIONAL(x)  \
    x == LESS_THAN        \
    || x == EQUALS        \
    || x == GREATER_THAN  \
    || x == NOT_EQ        \
    || x == LESS_EQ       \
    || x == GREATER_EQ

struct ast_node *comparison(struct parser *parse) {
    MPL_DEBUG(fprintf(stderr,
        "DEBUG: Calling comparison() on parser @ %p with current token \"%s\".\n",
        (void *)parse, parse->current_token->value
    ));

    struct ast_node *node = math_expression(parse);
    struct token *tok;

    while (IS_RELATIONAL(parse->current_token->type)) {
        tok = malloc(sizeof(struct token));
        construct_token(tok, parse->current_token->type, parse->current_token->value);
        eat(parse, tok->type);
        struct ast_node *left = node;
        node = malloc(sizeof(struct binary_op));
        struct ast_node *right = math_expression(parse);
        construct_binary_op((struct binary_op *)node, &left, tok, &right);
    }

    MPL_DEBUG(fprintf(stderr,
        "DEBUG:\t\tcomparison() is returning a new struct ast_node @ %p.\n", (void *)node));

    return node;
}


struct ast_node *parser_gen_ast(struct parser *parse) {
    // TODO implement the rest of the language
    parse->current_token = get_next_token(parse->lex);

    if (parse->current_token->type == END_OF_FILE) {
        free_token(parse->current_token);
        return NULL;
    }

    struct ast_node *result = expression(parse);
    free_token(parse->current_token);
    return result;
}
