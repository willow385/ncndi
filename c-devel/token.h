#ifndef TOKEN_H
#define TOKEN_H

/* This enum is used to distinguish different
   types of tokens. */
enum token_type {
    MULT = 0,
    PLUS = 1,
    INTEGER = 2,
    OPEN_PAREN = 3,
    CLOSE_PAREN = 4,
    DIVIDE = 5,
    SUBTRACT = 6,
    END_OF_FILE = 7,
    OPEN_BRACE = 8,
    CLOSE_BRACE = 9,
    SEMICOLON = 10,
    ASSIGN = 11,
    IDENTIFIER = 12,
    START = 13,
    END = 14,
    PRINT = 15,
    FLOAT = 16,
    TYPE_IDENTIFIER = 17,
    STRING = 18,
    FUNCTION_DECL = 19,
    COMMA = 20,
    RETURN = 21,
    IF = 22,
    ELSE = 23,
    GREATER_THAN = 24,
    LESS_THAN = 25,
    EQUALS = 26,
    AND = 27,
    OR = 28,
    XOR = 29,
    NOT = 30,
    NOT_EQ = 31,
    GREATER_EQ = 32,
    LESS_EQ = 33,
    WHILE = 34,
    FOR = 35,
    PLUS_ASSIGN = 36,
    SUBTRACT_ASSIGN = 37,
    MULT_ASSIGN = 38,
    DIVIDE_ASSIGN = 39,
    MODULUS = 40
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
