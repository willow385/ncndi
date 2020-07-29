#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import copy
from token import TokenType


# Base class representing abstract syntax tree nodes.
# Objects inheriting from ASTNode are intended to
# implement the eval() method. AST nodes which have
# children call eval() on their children whenever
# their own eval() method is called; thus, the calls
# to eval() methods *recursively descend* down the
# tree to evaluate the overall semantic meaning of
# the program.
# The parameters variable_scope and function_scope
# represent the variables and functions visible to
# the currently-evaluating AST node's scope.
class ASTNode:
    def eval(self, variable_scope: dict, function_scope: dict):
        raise NotImplementedError(f"Class {type(self).__name__} has not implemented the eval() method")


# Class representing binary operations.
# This includes arithmetic operations (*, /, +, -, %) and
# logical operations (==, !=, <, >, &&, ||, <>).
class BinaryOp(ASTNode):
    def __init__(self, left, op, right):
        self.left = left
        self.token = self.op = op
        self.right = right

    def __str__(self):
        if self.op.value == "==":
            return f"(eq {self.left} {self.right})"
        elif self.op.value == "!=" or self.op.value == "<>":
            return f"(not (eq {self.left} {self.right}))"
        elif self.op.value == "||":
            return f"(or {self.left.value} {self.right.value})"
        elif self.op.value == "&&":
            return f"(and {self.left.value} {self.right.value})"
        elif self.op.value == '%':
            return f"(mod {self.left.value} {self.right.value})"
        return f"({self.op.value} {self.left} {self.right})"

    def __repr__(self):
        return self.__str__()

    def eval(self, variable_scope: dict, function_scope: dict):
        # Evaluate the AST node on the left.
        left = self.left.eval(variable_scope, function_scope)

        # Evaluate the AST node on the right.
        right = self.right.eval(variable_scope, function_scope)

        # Check to see if they're both strings, if one of them is a string, or neither is a string.
        both_are_string = (type(left) is str and type(right) is str)
        one_is_a_string = (type(left) is not type(right)) and (str in [type(left), type(right)])

        # Identify which operation to perform. If the requested
        # operation is not implemented for the types of the
        # operands, throw an exception; MPL is a fairly strictly-
        # typed language.
        if self.op.token_type == TokenType.MULT:
            if one_is_a_string or both_are_string:
                raise Exception("Error: operator '*' not supported for string type")
            return left * right
        elif self.op.token_type == TokenType.PLUS:
            if one_is_a_string or both_are_string:
                # No type exception here. The '+' operator is
                # overloaded to perform string concatenation.
                return str(left) + str(right)
            return left + right
        elif self.op.token_type == TokenType.DIVIDE:
            if one_is_a_string or both_are_string:
                raise Exception("Error: operator '/' not supported for string type")
            if right == 0:
                raise Exception("Error: attempted to divide by zero")
            else:
                return left / right
        elif self.op.token_type == TokenType.SUBTRACT:
            if one_is_a_string or both_are_string:
                raise Exception("Error: operator '-' not supported for string type")
            return left - right
        elif self.op.token_type == TokenType.EQUALS:
            if one_is_a_string:
                return 0  # don't compare strings with other types
            return 1 if left == right else 0
        elif self.op.token_type == TokenType.GREATER_THAN:
            if one_is_a_string:
                return 0
            return 1 if left > right else 0
        elif self.op.token_type == TokenType.LESS_THAN:
            if one_is_a_string:
                return 0
            return 1 if left < right else 0
        elif self.op.token_type == TokenType.NOT_EQ:
            if one_is_a_string:
                return 0
            return 1 if left != right else 0
        elif self.op.token_type == TokenType.GREATER_EQ:
            if one_is_a_string:
                return 0
            return 1 if left >= right else 0
        elif self.op.token_type == TokenType.LESS_EQ:
            if one_is_a_string:
                return 0
            return 1 if left <= right else 0
        elif self.op.token_type == TokenType.AND:
            if one_is_a_string:
                return 0
            return 1 if (bool(left) and bool(right)) else 0
        elif self.op.token_type == TokenType.OR:
            if one_is_a_string:
                return 0
            return 1 if (bool(left) or bool(right)) else 0
        elif self.op.token_type == TokenType.XOR:
            if one_is_a_string:
                return 0
            # Why doesn't Python have logical xor?
            return 1 if bool(left) != bool(right) else 0
        elif self.op.token_type == TokenType.MODULUS:
            if one_is_a_string:
                return 0
            return left % right


# Class representing atomic values of the int type.
class Integer(ASTNode):
    def __init__(self, token):
        self.token = token
        self.value = token.value

    def __str__(self):
        return f"{self.value}"

    def __repr__(self):
        return self.__str__()

    def eval(self, variable_scope: dict, function_scope: dict):
        return int(self.value)


# Class representing atomic values of the float type.
class Float(ASTNode):
    def __init__(self, token):
        self.token = token
        self.value = token.value

    def __str__(self):
        return f"{self.value}"

    def __repr__(self):
        return self.__str__()

    def eval(self, variable_scope: dict, function_scope: dict):
        return float(self.value)


# Class representing atomic values of the string type.
class String(ASTNode):
    def __init__(self, value):
        self.value = value

    def __str__(self):
        return '"' + f'{self.value}'.replace(
            '\\', '\\\\'
        ).replace(
            '\n', '\\n'
        ).replace(
            '"', '\\"'
        ).replace(
            '\r', '\\r'
        ).replace(
            '\t', '\\t'
        ) + '"'

    def __repr__(self):
        return self.__str__()

    def eval(self, variable_scope: dict, function_scope: dict):
        return str(self.value)


# Class representing print statements.
class PrintStatement(ASTNode):
    def __init__(self, arg):
        self.arg = arg

    def __str__(self):
        return f'(format t "~a" {self.arg})'

    def __repr__(self):
        return self.__str__()

    def eval(self, variable_scope: dict, function_scope: dict):
        print(f"{self.arg.eval(variable_scope, function_scope)}", end="")


# The name of this class is slightly misleading.
# It represents any list of statements; for example,
# the body of a function or of a conditional statement.
class Program(ASTNode):
    def __init__(self):
        self.children = []

    def __str__(self):
        return " ".join(map(str, self.children))

    def __repr__(self):
        return self.__str__()

    def eval(self, variable_scope: dict, function_scope: dict):
        for node in self.children:
            result = node.eval(variable_scope, function_scope)
            if type(node) is ReturnStatement:
                return result
            elif type(node) in (IfStatement, WhileLoop, ForLoop):
                if result is not None:
                    return result


# Class representing variable assignments, consisting
# of declaring and assigning a variable in the same
# statement (for example, "int foo = 3;").
class Assignment(ASTNode):
    def __init__(self, var_type, identifier, op, value):
        self.var_type = var_type
        self.identifier = identifier
        self.token = self.op = op
        self.value = value

    def __str__(self):
        return f"(setq {self.identifier} {self.value})"

    def __repr__(self):
        return self.__str__()

    def eval(self, variable_scope: dict, function_scope: dict):
        if self.identifier.value in {**variable_scope, **function_scope}.keys():
            raise Exception(f"Error: {self.identifier.value} has multiple definitions")

        # Idk if anyone else uses this idiom but I find it
        # to be much more elegant and less verbose than a
        # chain of conditional statements.
        var_type = {
            "int": int,
            "float": float,
            "string": str
        }[self.var_type.value]

        variable_scope[self.identifier.value] = {
            "value": var_type(self.value.eval(variable_scope, function_scope)),
            "type": self.var_type.value
        }

        return variable_scope[self.identifier.value]["value"]


# Class representing variable declarations which don't
# specify an explicit value, for example "int foo;".
# By default, ints are initialized to 0, floats to 0.0,
# and strings to "".
class VariableDecl(ASTNode):
    def __init__(self, var_type, identifier):
        self.var_type = var_type
        self.identifier = identifier

    def __str__(self):
        return f"(setq {self.identifier} nil)"

    def __repr__(self):
        return self.__str__()

    def eval(self, variable_scope: dict, function_scope: dict):
        if self.identifier.value in {**variable_scope, **function_scope}.keys():
            raise Exception(f"Error: {self.identifier.value} has multiple definitions")
        value = None
        if self.var_type.value == "int":
            value = 0
        elif self.var_type.value == "float":
            value = 0.0
        elif self.var_type.value == "string":
            value = ""
        variable_scope[self.identifier.value] = {
            "value": value,
            "type": self.var_type.value
        }
        return variable_scope[self.identifier.value]["value"]


# Class representing reassignments; i.e. assigning a new
# value to an existing variable. Variables are statically
# typed, so if you try to assign a variable a value of a
# different type, it will be coerced to the type of the
# variable.
class Reassignment(ASTNode):
    def __init__(self, identifier, op, value):
        self.identifier = identifier
        self.token = self.op = op
        self.value = value

    def __str__(self):
        return f"(setq {self.identifier} {self.value})"

    def __repr__(self):
        return self.__str__()

    def eval(self, variable_scope: dict, function_scope: dict):
        var_id = self.identifier.value
        var_type = {
            "int": int,
            "float": float,
            "string": str
        }[variable_scope[var_id]["type"]]

        if self.op.token_type == TokenType.ASSIGN:
            variable_scope[var_id]["value"] = var_type(self.value.eval(variable_scope, function_scope))
        elif self.op.token_type == TokenType.PLUS_ASSIGN:
            variable_scope[var_id]["value"] += var_type(self.value.eval(variable_scope, function_scope))
        elif self.op.token_type == TokenType.SUBTRACT_ASSIGN:
            variable_scope[var_id]["value"] -= var_type(self.value.eval(variable_scope, function_scope))
        elif self.op.token_type == TokenType.MULT_ASSIGN:
            variable_scope[var_id]["value"] *= var_type(self.value.eval(variable_scope, function_scope))
        elif self.op.token_type == TokenType.DIVIDE_ASSIGN:
            if var_type is float:
                variable_scope[var_id]["value"] /= var_type(self.value.eval(variable_scope, function_scope))
            else:
                variable_scope[var_id]["value"] //= var_type(self.value.eval(variable_scope, function_scope))

        return variable_scope[var_id]["value"]


# Class representing variables.
class Variable(ASTNode):
    def __init__(self, token):
        self.token = token
        self.value = token.value

    def __str__(self):
        return f"{self.value}"

    def __repr__(self):
        return self.__str__()

    def eval(self, variable_scope: dict, function_scope: dict):
        return variable_scope[self.value]["value"]


# Class representing return statements.
class ReturnStatement(ASTNode):
    def __init__(self, return_value):
        self.value = return_value

    def __str__(self):
        return f"(return {self.value})"

    def __repr__(self):
        return self.__str__()

    def eval(self, variable_scope: dict, function_scope: dict):
        value = self.value.eval(variable_scope, function_scope)
        return value


# Class representing function declarations. Note that
# the body of the function is an instance of the Program
# class.
class Function(ASTNode):
    def __init__(self, function_name, function_params, return_type, function_body):
        self.function_name = function_name
        self.params = function_params
        self.return_type = return_type
        self.body = Program()
        self.body.children = function_body

    def __str__(self):
        result = f"(defun {self.function_name} ("
        if len(self.params) == 0:
            result += ")"
        elif len(self.params) == 1:
            result += f"{self.params[0].identifier})"
        else:
            result += f"{self.params[0].identifier}"
            for i in range(1, len(self.params)):
                result += f" {self.params[i].identifier}"
            result += ")"
        result += f" {self.body})"
        return result

    def __repr__(self):
        return self.__str__()

    def eval(self, variable_scope: dict, function_scope: dict):
        if self.function_name.value in {**variable_scope, **function_scope}.keys():
            # We don't allow multiple functions or variables with the same name
            # to reside in the same scope, ever.
            raise Exception(f"Error: {self.function_name.value} has multiple definitions")
        function_scope[self.function_name.value] = self


# Class representing a function call.
class FunctionCall(ASTNode):
    def __init__(self, function_id, args: list):
        self.func_id = function_id
        self.args = args
        self.local_scope = args

    def __str__(self):
        result = f"({self.func_id}"
        if len(self.args) == 0:
            result += ")"
        elif len(self.args) == 1:
            result += f" {self.args[0]})"
        else:
            result += f" {self.args[0]}"
            for i in range(1, len(self.args)):
                result += f" {self.args[i]}"
            result += ")"
        return result

    def __repr__(self):
        return self.__str__()

    def eval(self, variable_scope: dict, function_scope: dict):
        # We look up the function by its name to find out what
        # its parameters are supposed to be.
        params = function_scope[self.func_id.value].params
        args = self.args

        named_args = {}
        # Check to make sure that the number of args passed
        # matches the actual length of the parameter list.
        if len(params) != len(args):
            raise Exception(f"Error: incorrect number of arguments passed to {self.func_id.value}")

        # Populate our dict named_args with pairs mapping
        # the parameter names to their passed values.
        for i in range(len(params)):
            arg_type = {
                "int": int,
                "float": float,
                "string": str
            }[params[i].var_type.value]

            named_args[params[i].identifier.value] = {
                "value": arg_type(args[i].eval(variable_scope, function_scope)),
                "type": params[i].var_type.value
            }

        # Find the function's return type and obtain a result
        # if possible.
        ret_type = None
        if function_scope[self.func_id.value].return_type.value == "int":
            ret_type = int
        elif function_scope[self.func_id.value].return_type.value == "float":
            ret_type = float
        elif function_scope[self.func_id.value].return_type.value == "string":
            ret_type = str
        result = function_scope[self.func_id.value].body.eval(named_args, function_scope)
        if ret_type is not None:
            return ret_type(result)


# Class representing an if-statement.
class IfStatement(ASTNode):
    def __init__(self, body, condition, else_statement=None):
        self.body = Program()
        self.body.children = body
        self.condition = condition
        self.else_statement = else_statement

    def __str__(self):
        if self.else_statement is not None:
            return f"(if {self.condition} {self.body} {self.else_statement})"
        else:
            return f"(if {self.condition} {self.body})"

    def __repr__(self):
        return self.__str__()

    def eval(self, variable_scope: dict, function_scope: dict):
        # We want the body of the if-statement to be able to use local
        # variables that aren't visible to the scope outside the if-
        # statement. A better way to achieve this might be to have
        # the scope in which a variable is created be part of the
        # variable's metadata in variable_scope, but a quick-and-dirty
        # way to achieve it here is to copy the outer scope's variables,
        # let the new scope modify the copied dict, and then merge
        # the changed variables with the outer scope.
        local_var_scope = copy.deepcopy(variable_scope)

        # if-statements can be used as expressions in MPL, so we're
        # going to return a value
        return_val = None
        if self.condition.eval(variable_scope, function_scope) != 0:
            return_val = self.body.eval(local_var_scope, function_scope)
        # If the condition failed, and we have a following else-statement, then
        # execute that
        elif self.else_statement is not None:
            return_val = self.else_statement.eval(local_var_scope, function_scope)

        # Any outer-scope variables mutated? Copy the changes to the
        # outer scope's variables
        local_scope_var_ids = local_var_scope.keys()
        wider_scope_var_ids = variable_scope.keys()
        for var_id in local_scope_var_ids:
            if var_id in wider_scope_var_ids:
                variable_scope[var_id]["value"] = local_var_scope[var_id]["value"]

        return return_val


# Class representing an else-statement.
class ElseStatement(ASTNode):
    # If condition or else_statement is not None, then this else-
    # statement is actually an else-if-statement.
    def __init__(self, body, condition=None, else_statement=None):
        self.body = Program()
        self.body.children = body
        self.condition = condition
        self.else_statement = else_statement

    def __str__(self):
        result = ""
        if self.condition is not None:
            result += f"(if {self.condition} {self.body}"
            if self.else_statement is not None:
                result += f" {self.else_statement}"
            result += ")"
        else:
            result = f"{self.body}"
        return result

    def __repr__(self):
        return self.__str__()

    def eval(self, variable_scope: dict, function_scope: dict):
        # See comments in IfStatement.eval() for explanation
        local_var_scope = copy.deepcopy(variable_scope)
        return_val = None
        if self.condition is None:
            return_val = self.body.eval(local_var_scope, function_scope)
        elif self.condition.eval(variable_scope, function_scope) != 0:
            return_val = self.body.eval(local_var_scope, function_scope)
        # If we're in an else-if chain, pass control flow to the
        # next link in the chain
        elif self.else_statement is not None:
            return_val = self.else_statement.eval(variable_scope, function_scope)

        local_scope_var_ids = local_var_scope.keys()
        wider_scope_var_ids = variable_scope.keys()
        for var_id in local_scope_var_ids:
            if var_id in wider_scope_var_ids:
                variable_scope[var_id]["value"] = local_var_scope[var_id]["value"]

        return return_val


# Class for representing while-loops.
class WhileLoop(ASTNode):
    def __init__(self, body, condition):
        self.body = Program()
        self.body.children = body
        self.condition = condition

    def __str__(self):
        return f"(loop while {self.condition} do {self.body})"

    def __repr__(self):
        return self.__str__()

    def eval(self, variable_scope: dict, function_scope: dict):
        # See comments in IfStatement.eval() for explanation
        local_var_scope = copy.deepcopy(variable_scope)
        return_val = None
        time_to_stop = False
        while self.condition.eval(local_var_scope, function_scope) != 0 and not time_to_stop:
            local_var_scope = copy.deepcopy(variable_scope)
            return_val = self.body.eval(local_var_scope, function_scope)
            if return_val is not None:
                time_to_stop = True
            local_scope_var_ids = local_var_scope.keys()
            wider_scope_var_ids = variable_scope.keys()
            for var_id in local_scope_var_ids:
                if var_id in wider_scope_var_ids:
                    variable_scope[var_id]["value"] = local_var_scope[var_id]["value"]

        if return_val is not None:
            return return_val


# Class for representing for-loops.
class ForLoop(ASTNode):
    def __init__(self, body, initialization, condition, iteration):
        self.body = Program()
        self.body.children = body
        self.initialization = initialization
        self.condition = condition
        self.iteration = iteration

    def __str__(self):
        return f"(progn {self.initialization} (loop while {self.condition} do {self.body} {self.iteration}))"

    def __repr__(self):
        return self.__str__()

    def eval(self, variable_scope: dict, function_scope: dict):
        # See comments in IfStatement.eval() for explanation
        local_var_scope = copy.deepcopy(variable_scope)
        return_val = None
        time_to_stop = False

        # First evaluate the initialization
        self.initialization.eval(local_var_scope, function_scope)

        # Then start looping
        while self.condition.eval(local_var_scope, function_scope) != 0 and not time_to_stop:
            return_val = self.body.eval(local_var_scope, function_scope)
            if return_val is not None:
                time_to_stop = True

            # Run the iteration statement each loop
            self.iteration.eval(local_var_scope, function_scope)

            local_scope_var_ids = local_var_scope.keys()
            wider_scope_var_ids = variable_scope.keys()
            for var_id in local_scope_var_ids:
                if var_id in wider_scope_var_ids:
                    variable_scope[var_id]["value"] = local_var_scope[var_id]["value"]

        if return_val is not None:
            return return_val


# Class representing logical negation (the '!' operator).
class Negation(ASTNode):
    def __init__(self, operand):
        self.operand = operand

    def __str__(self):
        return f"(not {self.operand})"

    def __repr__(self):
        return self.__str__()

    def eval(self, variable_scope: dict, function_scope: dict):
        result = self.operand.eval(variable_scope, function_scope)
        if type(result) is str:
            return 0
        return int(not bool(result))


# Class that represents not doing anything. If you wrote an MPL
# program like "start{;;;;;;;}end", the interpreter would construct
# an AST filled with these.
class Nop(ASTNode):
    def __str__(self):
        return ""

    def __repr__(self):
        return self.__str__()

    def eval(self, variable_scope: dict, function_scope: dict):
        pass
