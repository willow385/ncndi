#include "token.h"

#ifndef LEXER_H
#define LEXER_H

/* Table of MPL reserved words */
extern const char *reserved_words[];

/* Number of reserved words in MPL */
extern size_t reserved_word_count;

/* The lexer is basically a simple finite state mechine. */
struct lexer {
    char *text;        // the text of the MPL source code to lex
    size_t text_size;  // number of chars in the source code
    size_t pos;        // index of currently selected char
    char current_char; // currently selected char
};

/**
 * Some identifiers happen to be reserved words. If an MPL
 * reserved word  is  passed to this function,  it returns
 * the corresponding token_type. For example, the function
 * call `identifier_type("funct")`  returns FUNCTION_DECL.
 * If an identifier which is not a reserved word is passed
 * it returns IDENTIFIER.
 *
 * @param identifier an MPL identifier
 * @return corresponding token_type for that identifier
 */
enum token_type identifier_type(const char *identifier);

/**
 * This function increments lex->pos.  If lex->pos is then
 * equal to lex->text_size,  lex->current_char  updates to
 * '\0';  else, lex->current_char updates to the next char
 * in lex->text.
 *
 * @param lex the lexer
 */
void advance(struct lexer *lex);

/**
 * This function advances the lexer repeatedly  until  the
 * current char is not a whitespace character.
 *
 * @param lex the lexer
 */
void skip_whitespace(struct lexer *lex);

/**
 * This function advances the lexer until lex->pos doesn't
 * point inside a comment anymore.
 *
 * @param lex the lexer
 */
void skip_comment(struct lexer *lex);

/**
 * This function parses a float or  integer  literal  from
 * the  lexer.  It returns  a  pointer to a heap-allocated
 * struct token,  which  must be freed by the caller using
 * the function free_token().
 *
 * @param lex the lexer
 * @return integer or float literal token
 */
struct token *parse_number(struct lexer *lex);

/**
 * This function  parses an identifier from the lexer.  It
 * too returns a pointer to memory that must be freed with
 * free_token().
 *
 * @param lex the lexer
 * @return identifier or reserved word token
 */
struct token *parse_identifier(struct lexer *lex);

/**
 * This function parses  a  string literal from the lexer.
 * Once again,  caller must call the function free_token()
 * on the returned pointer.
 *
 * @param lex the lexer
 * @return string literal token
 */
struct token *parse_string_literal(struct lexer *lex);

/**
 * This function returns the upcoming character.
 *
 * @param lex the lexer
 * @return lex->text[lex->pos+1] or '\0' if none left
 */
char peek(struct lexer *lex);

/**
 * This function returns the next token  from  the  lexer.
 * And guess what? That's right, the caller has to free it
 * with free_token()! Good job guessing, have a cigar.
 *
 * @param lex the lexer
 * @return next token in the program
 */
struct token *get_next_token(struct lexer *lex);

#endif // LEXER_H
