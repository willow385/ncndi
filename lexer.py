# TODO: implement modulo operator and bitwise operators

import string
from token import Token, TokenType

RESERVED_WORDS = {
    "start": Token(TokenType.START, "start"),
    "end": Token(TokenType.END, "end"),
    "print": Token(TokenType.PRINT, "print"),
    "int": Token(TokenType.TYPE_IDENTIFIER, "int"),
    "float": Token(TokenType.TYPE_IDENTIFIER, "float"),
    "string": Token(TokenType.TYPE_IDENTIFIER, "string"),
    "funct": Token(TokenType.FUNCTION_DECL, "funct"),
    "return": Token(TokenType.RETURN, "return"),
    "if": Token(TokenType.IF, "if"),
    "else": Token(TokenType.ELSE, "else"),
    "while": Token(TokenType.WHILE, "while"),
    "for": Token(TokenType.FOR, "for")
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
        while self.current_char is not None and (self.current_char.isdigit() or self.current_char == '.'):
            result += self.current_char
            self.advance()
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

    def parse_string_literal(self):
        result = ""
        if self.current_char == '"':
            self.advance()
        else:
            self.error("String literals must begin with '" '"' "'")

        while self.current_char != '"' or (self.current_char == '"' and self.text[self.pos-1] == '\\' and self.text[self.pos-2] != '\\'):
            result += self.current_char
            self.advance()
        self.advance()
        result = result.replace('\\n', '\n').replace('\\r', '\r').replace('\\t', '\t').replace('\\\\', '\\').replace("\\\"", "\"")
        return Token(TokenType.STRING, result)

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

            if self.current_char == ',':
                self.advance()
                return Token(TokenType.COMMA, ',')

            if self.current_char == '"':
                return self.parse_string_literal()

            if self.current_char == '#':
                self.skip_comment()
                return self.get_next_token()

            if self.current_char == '+':
                self.advance()
                if self.current_char == '=':
                    self.advance()
                    return Token(TokenType.PLUS_ASSIGN, "+=")
                return Token(TokenType.PLUS, '+')

            if self.current_char == '-':
                self.advance()
                if self.current_char == '=':
                    self.advance()
                    return Token(TokenType.SUBTRACT_ASSIGN, "-=")
                return Token(TokenType.SUBTRACT, '-')

            if self.current_char == '*':
                self.advance()
                if self.current_char == '=':
                    self.advance()
                    return Token(TokenType.MULT_ASSIGN, "*=")
                return Token(TokenType.MULT, '*')

            if self.current_char == '/':
                self.advance()
                if self.current_char == '=':
                    self.advance()
                    return Token(TokenType.DIVIDE_ASSIGN, "/=")
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
                if self.current_char == '=':
                    self.advance()
                    return Token(TokenType.EQUALS, "==")
                return Token(TokenType.ASSIGN, '=')

            if self.current_char == '>':
                self.advance()
                if self.current_char == '=':
                    self.advance()
                    return Token(TokenType.GREATER_EQ, ">=")
                return Token(TokenType.GREATER_THAN, '>')

            if self.current_char == '<':
                self.advance()
                if self.current_char == '>':
                    self.advance()
                    return Token(TokenType.XOR, "<>")
                elif self.current_char == '=':
                    self.advance()
                    return Token(TokenType.LESS_EQ, "<=")
                return Token(TokenType.LESS_THAN, '<')

            if self.current_char == '&':
                self.advance()
                if self.current_char == '&':
                    self.advance()
                    return Token(TokenType.AND, "&&")

            if self.current_char == '|':
                self.advance()
                if self.current_char == '|':
                    self.advance()
                    return Token(TokenType.OR, "||")

            if self.current_char == '!':
                self.advance()
                if self.current_char == '=':
                    self.advance()
                    return Token(TokenType.NOT_EQ, "!=")
                return Token(TokenType.NOT, '!')

            self.error(f"Syntax error at unexpected character '{self.current_char}'")

        return Token(TokenType.EOF, None)
