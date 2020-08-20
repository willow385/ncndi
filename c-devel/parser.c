#include "config.h"
#include <stdlib.h>
#include <stdio.h>
#include "token.h"
#include "lexer.h"
#include "ast_node.h"
#include "parser.h"

int eat(struct parser *parse, enum token_type type) {
    if (parse->current_token->type == type) {
        free_token(parse->current_token);
        parse->current_token = get_next_token(parse->lex);
        return 0;
    }

    free_token(parse->current_token);
    return 1;
}

struct mpl_program_block *program(struct parser *parse);

struct ast_node *factor(struct parser *parse) {
    struct token *tok = malloc(sizeof(struct token));
    construct_token(tok, parse->current_token->type, parse->current_token->value);

    switch (tok->type) {
        case INT_L:
            eat(parse, INT_L);
            struct mpl_object *result = malloc(sizeof(struct mpl_object));
            construct_mpl_object(result, INT, tok->value);
            free_token(tok);
            return (struct ast_node *)result;
        break;

        case FLOAT_L:
            eat(parse, FLOAT_L);
            struct mpl_object *result = malloc(sizeof(struct mpl_object));
            construct_mpl_object(result, FLOAT, tok->value);
            free_token(tok);
            return (struct ast_node *)result;
        break;

        case STRING_L:
            eat(parse, STRING_L);
            struct mpl_object *result = malloc(sizeof(struct mpl_object));
            construct_mpl_object(result, STRING, tok->value);
            free_token(tok);
            return (struct ast_node *)result;
        break;

        case OPEN_PAREN:
            eat(parse, OPEN_PAREN);
            struct ast_node *result = expression(parse);
            free_token(tok);
            eat(parse, CLOSE_PAREN);
            return result;
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
            // TODO return negative number
        break;

        default:
            fprintf(stderr, "Error: Unexpected token \"%s\" encountered\n", tok->value);
            free_token(tok);
        break;
    }

    return NULL;
}

struct ast_node *term(struct parser *parse) {
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

    return node;
}

struct ast_node *math_expression(struct parser *parse) {
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

    return node;
}

#define IS_LOGICAL_OP(x)  \
    x == AND              \
    || x == OR            \
    || x == XOR

struct ast_node *expression(struct parser *parse) {
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

    return node;
}

struct mpl_program_block *parser_gen_ast(struct parser *parse);
