#include "config.h"
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
 * This function returns the next  token  from  the lexer.
 * The token it returns is malloc'd and must be  freed  by
 * the caller using free_token().
 *
 * @param lex the lexer
 * @return next token in the program
 */
struct token *get_next_token(struct lexer *lex);

#endif // LEXER_H
