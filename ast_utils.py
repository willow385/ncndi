from token import Token, TokenType


class ASTNode:
    def eval(self, variable_scope: dict, function_scope: dict):
        raise NotImplementedError(f"Class {type(self).__name__} has not implemented the eval() method")


class BinaryOp(ASTNode):
    def __init__(self, left, op, right):
        self.left = left
        self.token = self.op = op
        self.right = right

    def __str__(self):
        return f"({self.left} {self.op.value} {self.right})"

    def __repr__(self):
        return self.__str__()

    def eval(self, variable_scope: dict, function_scope: dict):
        # Get the left operand
        left = self.left.eval(variable_scope, function_scope)

        # Get the right operand
        right = self.right.eval(variable_scope, function_scope)

        # Do an operation
        if self.op.token_type == TokenType.MULT:
            if type(left) is str or type(left) is str:
                raise Exception("Error: operator '*' not supported for string type")
            return left * right
        elif self.op.token_type == TokenType.PLUS:
            if type(left) is str or type(right) is str:
                return str(left) + str(right) # support string concatenation
            return left + right
        elif self.op.token_type == TokenType.DIVIDE:
            if type(left) is str or type(left) is str:
                raise Exception("Error: operator '/' not supported for string type")
            if right == 0:
                return float("inf")
            else:
                return left / right
        elif self.op.token_type == TokenType.SUBTRACT:
            if type(left) is str or type(left) is str:
                raise Exception("Error: operator '-' not supported for string type")
            return left - right



class Integer(ASTNode):
    def __init__(self, token):
        self.token = token
        self.value = token.value

    def __str__(self):
        return f"int({self.value})"

    def __repr__(self):
        return self.__str__()

    def eval(self, variable_scope: dict, function_scope: dict):
        return int(self.value)


class Float(ASTNode):
    def __init__(self, token):
        self.token = token
        self.value = token.value

    def __str__(self):
        return f"float({self.value})"

    def __repr__(self):
        return self.__str__()

    def eval(self, variable_scope: dict, function_scope: dict):
        return float(self.value)


class String(ASTNode):
    def __init__(self, value):
        self.value = value

    def __str__(self):
        return f'string("{self.value}")'.replace(
            '\\', '\\\\'
        ).replace(
            '\n', '\\n'
        ).replace(
            '"', '\\"'
        ).replace(
            '\r', '\\r'
        ).replace(
            '\t', '\\t'
        )

    def __repr__(self):
        return self.__str__()

    def eval(self, variable_scope: dict, function_scope: dict):
        return str(self.value)


class PrintStatement(ASTNode):
    def __init__(self, arg):
        self.arg = arg

    def __str__(self):
        return f"(print {self.arg})"

    def __repr__(self):
        return self.__str__()

    def eval(self, variable_scope: dict, function_scope: dict):
        print(f"{self.arg.eval(variable_scope, function_scope)}", end="")


class Program(ASTNode):
    def __init__(self):
        self.children = []

    def __str__(self):
        result = "("
        for node in self.children:
            result += str(node)
        result += ")"
        return result

    def __repr__(self):
        return self.__str__()

    def eval(self, variable_scope: dict, function_scope: dict):
        for node in self.children:
            result = node.eval(variable_scope, function_scope)
            if type(node) is ReturnStatement:
                return result


class Assignment(ASTNode):
    def __init__(self, var_type, identifier, op, value):
        self.var_type = var_type
        self.identifier = identifier
        self.token = self.op = op
        self.value = value

    def __str__(self):
        return f"({self.var_type} {self.identifier} = {self.value})"

    def __repr__(self):
        return self.__str__()

    def eval(self, variable_scope: dict, function_scope: dict):
        if self.identifier.value in {**variable_scope, **function_scope}.keys():
            raise Exception(f"Error: {self.identifier.value} has multiple definitions")
        var_type = {
            "int" : int,
            "float" : float,
            "string" : str
        }[self.var_type.value]
        variable_scope[self.identifier.value] = {
            "value" : var_type(self.value.eval(variable_scope, function_scope)),
            "type" : self.var_type.value
        }
        return variable_scope[self.identifier.value]["value"]


class VariableDecl(ASTNode):
    def __init__(self, var_type, identifier):
        self.var_type = var_type
        self.identifier = identifier

    def __str__(self):
        return f"({self.var_type} {self.identifier})"

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
            "value" : value,
            "type" : self.var_type.value
        }
        return variable_scope[self.identifier.value]["value"]


class Reassignment(ASTNode):
    def __init__(self, identifier, op, value):
        self.identifier = identifier
        self.token = self.op = op
        self.value = value

    def __str__(self):
        return f"({self.identifier} = {self.value})"

    def __repr__(self):
        return self.__str__()

    def eval(self, variable_scope: dict, function_scope: dict):
        var_type = None
        if variable_scope[self.identifier.value]["type"] == "int":
            var_type = int
        elif variable_scope[self.identifier.value]["type"] == "float":
            var_type = float
        elif variable_scope[self.identifier.value]["type"] == "string":
            var_type = str
        variable_scope[self.identifier.value]["value"] = var_type(self.value.eval(variable_scope, function_scope))


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


class Function(ASTNode):
    def __init__(self, function_name, function_params, return_type, function_body):
        self.function_name = function_name
        self.params = function_params
        self.return_type = return_type
        self.body = Program()
        self.body.children = function_body

    def __str__(self):
        result = f"(function {self.function_name}("
        if len(self.params) == 0:
            result += ")"
        elif len(self.params) == 1:
            result += f"{self.params[0]})"
        else:
            result += f"{self.params[0]}"
            for i in range(1, len(self.params)):
                result += f", {self.params[i]}"
            result += ")"
        result += f": {self.return_type} = ({self.body})"
        return result

    def __repr__(self):
        return self.__str__()

    def eval(self, variable_scope: dict, function_scope: dict):
        if self.function_name.value in {**variable_scope, **function_scope}.keys():
            raise Exception(f"Error: {self.function_name.value} has multiple definitions")
        function_scope[self.function_name.value] = self


class FunctionCall(ASTNode):
    def __init__(self, function_id, args: list):
        self.func_id = function_id
        self.args = args
        self.local_scope = args

    def __str__(self):
        result = f"(call to {self.func_id}("
        if len(self.args) == 0:
            result += ")"
        elif len(self.args) == 1:
            result += f"{self.args[0]})"
        else:
            result += f"{self.args[0]}"
            for i in range(1, len(self.args)):
                result += f", {self.args[i]}"
            result += ")"
        result += ")"
        return result

    def __repr__(self):
        return self.__str__()

    def eval(self, variable_scope: dict, function_scope: dict):
        params = function_scope[self.func_id.value].params
        args = self.args

        named_args = {}
        if len(params) != len(args):
            raise Exception(f"Error: incorrect number of arguments passed to {self.func_id.value}")

        for i in range(len(params)):
            arg_type = {
                "int" : int,
                "float" : float,
                "string" : str
            }[params[i].var_type.value]

            named_args[params[i].identifier.value] = {
                "value" : arg_type(args[i].eval(variable_scope, function_scope)),
                "type" : params[i].var_type.value
            }

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


class Nop(ASTNode):
    def __str__(self):
        return ""

    def __repr__(self):
        return self.__str__()

    def eval(self, variable_scope: dict, function_scope: dict):
        pass
