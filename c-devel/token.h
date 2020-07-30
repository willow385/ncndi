#ifndef TOKEN_H
#define TOKEN_H

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


struct token {
    enum token_type type;
    char *value;
};

void free_token(struct token *t);

#endif // TOKEN_H
