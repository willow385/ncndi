import string
from token import Token, TokenType

RESERVED_WORDS = {
    "start": Token(TokenType.START, "start"),
    "end": Token(TokenType.END, "end"),
    "print": Token(TokenType.PRINT, "print"),
    "int": Token(TokenType.TYPE_IDENTIFIER, "int"),
    "float": Token(TokenType.TYPE_IDENTIFIER, "float"),
    "string": Token(TokenType.TYPE_IDENTIFIER, "string")
}

class Lexer:
    def __init__(self, text):
        self.text = text
        self.pos = 0
        self.current_char = self.text[self.pos]

    def error(self, message):
        raise Exception("Error while lexing: " + message)

    def advance(self):
        self.pos += 1
        if self.pos > len(self.text) - 1:
            self.current_char = None
        else:
            self.current_char = self.text[self.pos]

    def skip_whitespace(self):
        while self.current_char is not None and self.current_char.isspace():
            self.advance()

    def parse_number(self):
        result = ""
        if self.current_char == '-': # handle negative numbers
            result += self.current_char
            self.advance()
        while self.current_char is not None and (self.current_char.isdigit() or self.current_char == '.'):
            result += self.current_char
            self.advance()
        if result == "-":
            self.error("Expected integer literal, got '-' instead")
        else:
            if '.' in result:
                return float(result)
            else:
                return int(result)

    def skip_comment(self):
        if self.current_char == '#':
            while self.current_char not in ('\r', '\n'):
                self.advance()
        while self.current_char in ('\r', '\n'):
            self.advance()

    def parse_identifier(self):
        result = ""
        if self.current_char in string.ascii_letters + '_':
            result += self.current_char
            self.advance()
        else:
            self.error("Identifiers can't start with " + self.current_char)
        while self.current_char is not None and self.current_char in string.ascii_letters + string.digits + '_':
            result += self.current_char
            self.advance()
        if result in RESERVED_WORDS.keys():
            return RESERVED_WORDS[result]
        else:
            return Token(TokenType.IDENTIFIER, result)

    def peek(self):
        if self.pos + 1 >= len(self.text):
            return None
        else:
            return self.text[self.pos + 1]

    def get_number_token(self):
        value = self.parse_number()
        if type(value) is float:
            return Token(TokenType.FLOAT, value)
        else:
            return Token(TokenType.INTEGER, value)

    def get_next_token(self):
        while self.current_char is not None:

            if self.current_char in string.ascii_letters + '_':
                return self.parse_identifier()

            if self.current_char.isspace():
                self.skip_whitespace()
                continue

            if self.current_char.isdigit():
                return self.get_number_token()

            if self.current_char == '#':
                self.skip_comment()
                return self.get_next_token()

            if self.current_char == '+':
                self.advance()
                return Token(TokenType.PLUS, '+')

            if self.current_char == '-':
                if self.pos == 0:
                    return self.get_number_token()
                elif self.text[self.pos - 1] in ('-', '+', '*', '/', '('):
                    return self.get_number_token()
                else:
                    self.advance()
                    return Token(TokenType.SUBTRACT, '-')

            if self.current_char == '*':
                self.advance()
                return Token(TokenType.MULT, '*')

            if self.current_char == '/':
                self.advance()
                return Token(TokenType.DIVIDE, '/')

            if self.current_char == '(':
                self.advance()
                return Token(TokenType.OPEN_PAREN, '(')

            if self.current_char == ')':
                self.advance()
                return Token(TokenType.CLOSE_PAREN, ')')

            if self.current_char == '{':
                self.advance()
                return Token(TokenType.OPEN_BRACE, '{')

            if self.current_char == '}':
                self.advance()
                return Token(TokenType.CLOSE_BRACE, '}')

            if self.current_char == ';':
                self.advance()
                return Token(TokenType.SEMICOLON, ';')

            if self.current_char == '=':
                self.advance()
                return Token(TokenType.ASSIGN, '=')

            self.error(f"Syntax error at unexpected character '{self.current_char}'")

        return Token(TokenType.EOF, None)
