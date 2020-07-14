from enum import Enum

class TokenType(Enum):
    MULT = 0
    PLUS = 1
    INTEGER = 2
    OPEN_PAREN = 3
    CLOSE_PAREN = 4
    DIVIDE = 5
    SUBTRACT = 6
    EOF = 7
    OPEN_BRACE = 8
    CLOSE_BRACE = 9
    SEMICOLON = 10
    ASSIGN = 11
    IDENTIFIER = 12
    START = 13
    END = 14
    PRINT = 15
    FLOAT = 16
    TYPE_IDENTIFIER = 17
    STRING = 18
    FUNCTION_DECL = 19
    COMMA = 20
    RETURN = 21
    IF = 22
    ELSE = 23
    GREATER_THAN = 24
    LESS_THAN = 25
    EQUALS = 26

class Token:
    def __init__(self, token_type, value):
        self.token_type = token_type
        self.value = value

    def __str__(self):
        return f"{self.value}"

    def __repr__(self):
        return self.__str__()
