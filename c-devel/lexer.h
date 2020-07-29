#include "token.h"

#ifndef LEXER_H
#define LEXER_H

extern const char *reserved_words[];

extern size_t reserved_word_count;

struct lexer {
    char *text;
    size_t text_size;
    size_t pos;
    char current_char;
};

enum token_type identifier_type(const char *identifier);

/* These three functions  all mutate  lex->pos
   in some way, but leave the other members of
   lex alone.  */
void advance(struct lexer *lex);
void skip_whitespace(struct lexer *lex);
void skip_comment(struct lexer *lex);

/* The functions  here that  return  struct token pointers
   all use  malloc() internally. The caller is responsible
   for freeing these. REMEMBER to free token->value BEFORE
   you free token! */
struct token *parse_number(struct lexer *lex);
struct token *parse_identifier(struct lexer *lex);
struct token *parse_string_literal(struct lexer *lex);

/* See what the next character is. */
char peek(struct lexer *lex);

/* Get the next token. This too is malloc'd and must be
   freed  by the caller.  Once again,  remember to free
   token->value before freeing token. */
struct token *get_next_token(struct lexer *lex);

#endif // LEXER_H
