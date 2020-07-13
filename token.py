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

class Token:
    def __init__(self, token_type, value):
        self.token_type = token_type
        self.value = value
