#include "token.h"

#ifndef LEXER_H
#define LEXER_H

#define IS_WHITESPACE(

const char *reserved_words[] = {
    "start",
    "end",
    "print",
    "int",
    "float",
    "string",
    "funct",
    "return",
    "if",
    "else",
    "while",
    "for"
};

size_t reserved_word_count = sizeof(reserved_words) / sizeof(char *);

struct lexer {
    char *text;
    size_t text_size;
    size_t pos;
    char current_char;
};

enum token_type reserved_word_type(const char *reserved_word);

int is_reserved_word(const char *string);

/* Print an error message to stderr. */
void lexer_error(const char *error_message);

/* These three functions all mutate lex->pos
   in some way. */
void advance(struct lexer *lex);
void skip_whitespace(struct lexer *lex);
void skip_comment(struct lexer *lex);

/* The functions here that return struct token pointers
   all use malloc() internally. The caller is responsible
   for freeing these. */
struct token *parse_number(struct lexer *lex);
struct token *parse_identifier(struct lexer *lex);
struct token *parse_string_literal(struct lexer *lex);

/* See what the next character is. */
char peek(struct lexer *lex);

/* Get the next token. This too is malloc'd and must be
   freed by the caller. */
struct token *get_next_token(struct lexer *lex);

#endif // LEXER_H
