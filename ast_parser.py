#!/usr/bin/env python3
# -*- coding: utf-8 -*-
from ast_utils import *
from token import *
from lexer import Lexer


class Parser:

    def __init__(self, lexer: Lexer):
        self.lexer = lexer
        self.current_token = self.lexer.get_next_token()
        self.has_just_parsed_block_statement = False

    def error(self, message):
        raise Exception("Error while parsing: " + message)

    def eat(self, token_type):
        if self.current_token.token_type == token_type:
            self.current_token = self.lexer.get_next_token()
        else:
            self.error(f"Expected {token_type}, got {self.current_token.token_type}")

    def program(self):
        nodes = []
        if self.current_token.token_type == TokenType.FUNCTION_DECL:
            nodes += self.function_list()
        self.eat(TokenType.START)
        self.eat(TokenType.OPEN_BRACE)
        nodes += self.statement_list()
        self.eat(TokenType.CLOSE_BRACE)
        self.eat(TokenType.END)
        root = Program()
        for node in nodes:
            root.children.append(node)
        return root

    def function_list(self):
        result = []
        while self.current_token.token_type != TokenType.START:
            result.append(self.function_declaration())
        return result

    def function_declaration(self):
        self.eat(TokenType.FUNCTION_DECL)
        function_name = self.current_token
        self.eat(TokenType.IDENTIFIER)
        self.eat(TokenType.OPEN_PAREN)
        function_params = self.parameter_list()
        self.eat(TokenType.CLOSE_PAREN)
        if self.current_token.token_type == TokenType.TYPE_IDENTIFIER:
            return_type = self.current_token
            self.eat(TokenType.TYPE_IDENTIFIER)
        else:
            return_type = Token(TokenType.TYPE_IDENTIFIER, "void")
        self.eat(TokenType.OPEN_BRACE)
        function_body = self.statement_list()
        self.eat(TokenType.CLOSE_BRACE)
        return Function(function_name, function_params, return_type, function_body)

    def parameter_list(self):
        node = []
        if self.current_token.token_type == TokenType.CLOSE_PAREN:
            return node
        first_param_type = self.current_token
        self.eat(TokenType.TYPE_IDENTIFIER)
        first_param_identifier = self.current_token
        self.eat(TokenType.IDENTIFIER)
        node.append(VariableDecl(first_param_type, first_param_identifier))
        while self.current_token.token_type != TokenType.CLOSE_PAREN:
            self.eat(TokenType.COMMA)
            param_type = self.current_token
            self.eat(TokenType.TYPE_IDENTIFIER)
            param_identifier = self.current_token
            self.eat(TokenType.IDENTIFIER)
            node.append(VariableDecl(param_type, param_identifier))
        return node

    def argument_list(self):
        node = []
        if self.current_token.token_type == TokenType.CLOSE_PAREN:
            return node
        first_arg = self.expression()
        node.append(first_arg)
        while self.current_token.token_type != TokenType.CLOSE_PAREN:
            self.eat(TokenType.COMMA)
            arg = self.expression()
            node.append(arg)
        return node

    def statement_list(self):
        result = [self.statement()]
        while self.current_token.token_type == TokenType.SEMICOLON or self.has_just_parsed_block_statement:
            if self.current_token.token_type == TokenType.SEMICOLON:
                self.eat(TokenType.SEMICOLON)
            elif self.has_just_parsed_block_statement:
                self.has_just_parsed_block_statement = False
            result.append(self.statement())
        if self.current_token.token_type == TokenType.IDENTIFIER:
            self.error(f"Syntax error at unexpected token ``{self.current_token.value}''")
        return result

    def statement(self):
        node = None
        if self.current_token.token_type == TokenType.TYPE_IDENTIFIER:
            node = self.assignment_statement()
        elif self.current_token.token_type == TokenType.IDENTIFIER:
            token_id = self.current_token
            self.eat(TokenType.IDENTIFIER)
            assign_ops = (
                TokenType.ASSIGN,
                TokenType.PLUS_ASSIGN,
                TokenType.SUBTRACT_ASSIGN,
                TokenType.MULT_ASSIGN,
                TokenType.DIVIDE_ASSIGN
            )
            if self.current_token.token_type in assign_ops:
                node = self.reassignment_statement(token_id)
            elif self.current_token.token_type == TokenType.OPEN_PAREN:
                node = self.function_call(token_id)
        elif self.current_token.token_type == TokenType.PRINT:
            node = self.print_statement()
        elif self.current_token.token_type == TokenType.RETURN:
            node = self.return_statement()
        elif self.current_token.token_type == TokenType.IF:
            node = self.if_statement()
        elif self.current_token.token_type == TokenType.WHILE:
            node = self.while_loop()
        elif self.current_token.token_type == TokenType.FOR:
            node = self.for_loop()
        else:
            node = self.empty()
        return node

    def if_statement(self):
        self.eat(TokenType.IF)
        condition = self.expression()
        self.eat(TokenType.OPEN_BRACE)
        body = self.statement_list()
        self.eat(TokenType.CLOSE_BRACE)
        else_clause = None
        if self.current_token.token_type == TokenType.ELSE:
            else_clause = self.else_statement()
        node = IfStatement(body, condition, else_clause)
        self.has_just_parsed_block_statement = True
        return node

    def else_statement(self):
        self.eat(TokenType.ELSE)
        condition = None
        else_clause = None
        if self.current_token.token_type == TokenType.IF:
            self.eat(TokenType.IF)
            condition = self.expression()
        self.eat(TokenType.OPEN_BRACE)
        body = self.statement_list()
        self.eat(TokenType.CLOSE_BRACE)
        if self.current_token.token_type == TokenType.ELSE:
            else_clause = self.else_statement()
        node = ElseStatement(body, condition, else_clause)
        return node

    def while_loop(self):
        self.eat(TokenType.WHILE)
        condition = self.expression()
        self.eat(TokenType.OPEN_BRACE)
        body = self.statement_list()
        self.eat(TokenType.CLOSE_BRACE)
        node = WhileLoop(body, condition)
        self.has_just_parsed_block_statement = True
        return node

    def for_loop(self):
        self.eat(TokenType.FOR)
        if self.current_token.token_type == TokenType.SEMICOLON:
            initialization = Nop()
        else:
            initialization = self.statement()
        self.eat(TokenType.SEMICOLON)
        if self.current_token.token_type == TokenType.SEMICOLON:
            condition = Integer(Token(TokenType.INTEGER, 1))
        else:
            condition = self.expression()
        self.eat(TokenType.SEMICOLON)
        if self.current_token.token_type == TokenType.OPEN_BRACE:
            iteration = Nop()
        else:
            iteration = self.statement()
        self.eat(TokenType.OPEN_BRACE)
        body = self.statement_list()
        self.eat(TokenType.CLOSE_BRACE)
        node = ForLoop(body, initialization, condition, iteration)
        self.has_just_parsed_block_statement = True
        return node

    def function_call(self, func_id):
        self.eat(TokenType.OPEN_PAREN)
        arguments = self.argument_list()
        self.eat(TokenType.CLOSE_PAREN)
        node = FunctionCall(func_id, arguments)
        return node

    def return_statement(self):
        self.eat(TokenType.RETURN)
        return_val = self.expression()
        return ReturnStatement(return_val)

    def assignment_statement(self):
        var_type = self.current_token
        self.eat(TokenType.TYPE_IDENTIFIER)
        var_identifier = self.current_token
        self.eat(TokenType.IDENTIFIER)
        token = self.current_token
        if token.token_type == TokenType.SEMICOLON:
            node = VariableDecl(var_type, var_identifier)
            return node
        elif token.token_type == TokenType.ASSIGN:
            self.eat(TokenType.ASSIGN)
            value = self.expression()
            return Assignment(var_type, var_identifier, token, value)
        self.error(f"Expected semicolon or assignment, got {token} instead")

    def reassignment_statement(self, id_token):
        token = self.current_token
        assign_ops = (
            TokenType.ASSIGN,
            TokenType.PLUS_ASSIGN,
            TokenType.SUBTRACT_ASSIGN,
            TokenType.MULT_ASSIGN,
            TokenType.DIVIDE_ASSIGN
        )
        if self.current_token.token_type in assign_ops:
            self.eat(self.current_token.token_type)
        else:
            self.error(
                f"Unexpected token ``{self.current_token}'' encountered, "
                "expected one of =, +=, -=, *=, or /="
            )
        value = self.expression()
        node = Reassignment(id_token, token, value)
        return node

    def print_statement(self):
        self.eat(TokenType.PRINT)
        value = self.expression()
        node = PrintStatement(value)
        return node

    @staticmethod
    def empty():
        return Nop()

    def factor(self):
        token = self.current_token
        if token.token_type == TokenType.INTEGER:
            self.eat(TokenType.INTEGER)
            return Integer(token)
        elif token.token_type == TokenType.FLOAT:
            self.eat(TokenType.FLOAT)
            return Float(token)
        elif token.token_type == TokenType.OPEN_PAREN:
            self.eat(TokenType.OPEN_PAREN)
            node = self.expression()
            self.eat(TokenType.CLOSE_PAREN)
            return node
        elif token.token_type == TokenType.IDENTIFIER:
            identifier = token
            self.eat(TokenType.IDENTIFIER)
            if self.current_token.token_type == TokenType.OPEN_PAREN:
                self.eat(TokenType.OPEN_PAREN)
                args = self.argument_list()
                self.eat(TokenType.CLOSE_PAREN)
                node = FunctionCall(identifier, args)
            else:
                node = Variable(identifier)
            return node
        elif token.token_type == TokenType.STRING:
            self.eat(TokenType.STRING)
            return String(token)
        elif token.token_type == TokenType.IF:
            node = self.if_statement()
            return node
        elif token.token_type == TokenType.NOT:
            self.eat(TokenType.NOT)
            operand = self.expression()
            return Negation(operand)
        elif token.token_type == TokenType.SUBTRACT:
            self.eat(TokenType.SUBTRACT)
            number = self.factor()
            number.value = -number.value
            return number

    def term(self):
        node = self.factor()

        while self.current_token.token_type in (TokenType.MULT, TokenType.DIVIDE, TokenType.MODULUS):
            token = self.current_token
            if token.token_type == TokenType.MULT:
                self.eat(TokenType.MULT)
            elif token.token_type == TokenType.DIVIDE:
                self.eat(TokenType.DIVIDE)
            elif token.token_type == TokenType.MODULUS:
                self.eat(TokenType.MODULUS)
            node = BinaryOp(left=node, op=token, right=self.factor())

        return node

    def math_expression(self):
        node = self.term()

        while self.current_token.token_type in (TokenType.PLUS, TokenType.SUBTRACT):
            token = self.current_token
            if token.token_type == TokenType.PLUS:
                self.eat(TokenType.PLUS)
            elif token.token_type == TokenType.SUBTRACT:
                self.eat(TokenType.SUBTRACT)

            node = BinaryOp(left=node, op=token, right=self.term())

        return node

    def expression(self):
        node = self.comparison()

        ops = (TokenType.AND, TokenType.OR, TokenType.XOR)
        while self.current_token.token_type in ops:
            token = self.current_token
            if token.token_type == TokenType.AND:
                self.eat(TokenType.AND)
            elif token.token_type == TokenType.OR:
                self.eat(TokenType.OR)
            elif token.token_type == TokenType.XOR:
                self.eat(TokenType.XOR)

            node = BinaryOp(left=node, op=token, right=self.comparison())

        return node

    def comparison(self):
        node = self.math_expression()

        bool_expressions = (
            TokenType.LESS_THAN,
            TokenType.EQUALS,
            TokenType.GREATER_THAN,
            TokenType.NOT_EQ,
            TokenType.LESS_EQ,
            TokenType.GREATER_EQ
        )

        while self.current_token.token_type in bool_expressions:
            token = self.current_token
            self.eat(token.token_type)
            node = BinaryOp(left=node, op=token, right=self.math_expression())

        return node

    def parse(self):
        node = self.program()
        if self.current_token.token_type != TokenType.EOF:
            self.error(f"Expected EOF, got ``{self.current_token.value}''")
        return node
