#include "config.h"

#ifndef TOKEN_H
#define TOKEN_H

/* This enum is used to distinguish different
   types of tokens. */
enum token_type {
    END_OF_FILE = -1,
    START = 0,
    END = 1,
    PRINT = 2,
    INT_L = 3,
    FLOAT_L = 4,
    STRING_L = 5,
    FUNCTION_DECL = 6,
    RETURN = 7,
    IF = 8,
    ELSE = 9,
    WHILE = 10,
    FOR = 11,
    MULT,
    PLUS,
    OPEN_PAREN,
    CLOSE_PAREN,
    DIVIDE,
    SUBTRACT,
    OPEN_BRACE,
    CLOSE_BRACE,
    SEMICOLON,
    ASSIGN,
    IDENTIFIER,
    TYPE_IDENTIFIER,
    COMMA,
    GREATER_THAN,
    LESS_THAN,
    EQUALS,
    AND,
    OR,
    XOR,
    NOT,
    NOT_EQ,
    GREATER_EQ,
    LESS_EQ,
    PLUS_ASSIGN,
    SUBTRACT_ASSIGN,
    MULT_ASSIGN,
    DIVIDE_ASSIGN,
    MODULUS
};

/* When we lex the source code, we convert it from
   an array of char  into  an array of these guys.
   This makes it much easier  to  parse  the  code
   into an abstract syntax tree. */
struct token {
    enum token_type type;
    char *value;
};

/* Whenever we create a  struct token on the heap,
   we have to allocate memory  for token->value as
   well as for the token itself.  So every time we
   want to deallocate a  struct token,  we have to
   free  token->value  *before* we can safely free
   token itself. This function does just that. */
void free_token(struct token *t);

/* Populate a token's members in place */
void construct_token(
    struct token *dest,
    enum token_type type,
    const char *value
);

/* Change a token's value */
void change_token_value(
    struct token *dest,
    const char *value
);

#endif // TOKEN_H
