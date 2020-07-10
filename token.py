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

class Token:
    def __init__(self, token_type, value):
        self.token_type = token_type
        self.value = value
