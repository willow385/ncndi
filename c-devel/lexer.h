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

/* Get the token type corresponding to an identifier.
   If the identifier is a reserved word,  returns the
   appropriate token_type for that word; else, simply
   returns IDENTIFIER. */
enum token_type identifier_type(const char *identifier);

/* These three functions all  mutate  lex->pos
   in some way, but leave the other members of
   lex alone.  */
void advance(struct lexer *lex);
void skip_whitespace(struct lexer *lex);
void skip_comment(struct lexer *lex);

/* The functions  here that  return  struct token pointers
   all use  malloc() internally. The caller is responsible
   for freeing these. Use the function free_token(), which
   is declared in token.h and defined in token.c,  to free
   the structs token. */
struct token *parse_number(struct lexer *lex);
struct token *parse_identifier(struct lexer *lex);
struct token *parse_string_literal(struct lexer *lex);

/* See what the next character is. */
char peek(struct lexer *lex);

/* Get the next token. This too is malloc'd and must be
   freed  by the caller.  Once again,  remember to free
   using free_token(). */
struct token *get_next_token(struct lexer *lex);

#endif // LEXER_H
