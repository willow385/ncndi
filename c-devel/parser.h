#include "config.h"
#include <stdlib.h>
#include <stdio.h>
#include "token.h"
#include "lexer.h"
#include "ast_node.h"

/* The parser does not own *lex, but it does own *current_token
   and is responsible for managing it. */
struct parser {
    struct lexer *lex;
    struct token *current_token;
    int just_parsed_program_block;
};

int eat(struct parser *parse, enum token_type type);

struct mpl_program_block *program(struct parser *parse);

struct ast_node *factor(struct parser *parse);

struct ast_node *term(struct parser *parse);

struct ast_node *math_expression(struct parser *parse);

struct ast_node *expression(struct parser *parse);

struct ast_node *comparison(struct parser *parse);

struct ast_node *parser_gen_ast(struct parser *parse);
