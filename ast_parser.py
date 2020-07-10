from ast_utils import *
from lexer import Lexer

class Parser:
    def __init__(self, lexer):
        self.lexer = lexer
        self.current_token = self.lexer.get_next_token()

    def error(self, message):
        raise Exception("Error while parsing: " + message)

    def eat(self, token_type):
        if self.current_token.token_type == token_type:
            self.current_token = self.lexer.get_next_token()
        else:
            self.error(f"Expected {token_type}, got {self.current_token.token_type}")

    def factor(self):
        token = self.current_token
        if token.token_type == TokenType.INTEGER:
            self.eat(TokenType.INTEGER)
            return Integer(token)
        elif token.token_type == TokenType.OPEN_PAREN:
            self.eat(TokenType.OPEN_PAREN)
            node = self.expr()
            self.eat(TokenType.CLOSE_PAREN)
            return node

    def term(self):
        node = self.factor()

        while self.current_token.token_type in (TokenType.MULT, TokenType.DIVIDE):
            token = self.current_token
            if token.token_type == TokenType.MULT:
                self.eat(TokenType.MULT)
            elif token.token_type == TokenType.DIVIDE:
                self.eat(TokenType.DIVIDE)
            node = BinaryOp(left=node, op=token, right=self.factor())

        return node

    def expr(self):
        node = self.term()

        while self.current_token.token_type in (TokenType.PLUS, TokenType.SUBTRACT):
            token = self.current_token
            if token.token_type == TokenType.PLUS:
                self.eat(TokenType.PLUS)
            elif token.token_type == TokenType.SUBTRACT:
                self.eat(TokenType.SUBTRACT)

            node = BinaryOp(left=node, op=token, right=self.term())

        return node

    def parse(self):
        return self.expr()
