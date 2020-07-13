from ast_utils import *
from lexer import Lexer

class Parser:
    def __init__(self, lexer):
        self.lexer = lexer
        self.global_scope = {}
        self.current_token = self.lexer.get_next_token()

    def error(self, message):
        raise Exception("Error while parsing: " + message)

    def eat(self, token_type):
        if self.current_token.token_type == token_type:
            self.current_token = self.lexer.get_next_token()
        else:
            self.error(f"Expected {token_type}, got {self.current_token.token_type}")

    def program(self):
        self.eat(TokenType.START)
        self.eat(TokenType.OPEN_BRACE)
        nodes = self.statement_list()
        self.eat(TokenType.CLOSE_BRACE)
        self.eat(TokenType.END)
        root = Program()
        for node in nodes:
            root.children.append(node)
        return root

    def statement_list(self):
        result = []
        result.append(self.statement())
        while self.current_token.token_type == TokenType.SEMICOLON:
            self.eat(TokenType.SEMICOLON)
            result.append(self.statement())
        if self.current_token.token_type == TokenType.IDENTIFIER:
            self.error(f"Syntax error at unexpected token ``{self.current_token.value}''")
        return result

    def statement(self):
        node = None
        if self.current_token.token_type == TokenType.IDENTIFIER:
            node = self.assignment_statement()
        elif self.current_token.token_type == TokenType.PRINT:
            node = self.print_statement()
        else:
            node = self.empty()
        return node

    def assignment_statement(self):
        var_ident = self.variable()
        token = self.current_token
        self.eat(TokenType.ASSIGN)
        value = self.expr()
        node = Assignment(var_ident, token, value)
        return node

    def print_statement(self):
        self.eat(TokenType.PRINT)
        value = self.expr()
        node = PrintStatement(value)
        return node

    def variable(self):
        node = Variable(self.current_token)
        self.eat(TokenType.IDENTIFIER)
        return node

    def empty(self):
        return Nop()

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
        elif token.token_type == TokenType.IDENTIFIER:
            node = self.variable()
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
        node = self.program()
        if self.current_token.token_type != TokenType.EOF:
            self.error(f"Expected EOF, got ``{self.current_token.value}''")
        return node
