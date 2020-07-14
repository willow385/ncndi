from token import Token, TokenType


class ASTNode:
    pass


class BinaryOp(ASTNode):
    def __init__(self, left, op, right):
        self.left = left
        self.token = self.op = op
        self.right = right

    def __str__(self):
        return f"({self.left} {self.op.value} {self.right})"

    def __repr__(self):
        return self.__str__()


class Integer(ASTNode):
    def __init__(self, token):
        self.token = token
        self.value = token.value

    def __str__(self):
        return f"int({self.value})"

    def __repr__(self):
        return self.__str__()


class Float(ASTNode):
    def __init__(self, token):
        self.token = token
        self.value = token.value

    def __str__(self):
        return f"float({self.value})"

    def __repr__(self):
        return self.__str__()


class String(ASTNode):
    def __init__(self, value):
        self.value = value

    def __str__(self):
        return f'string("{self.value}")'

    def __repr__(self):
        return self.__str__()


class PrintStatement(ASTNode):
    def __init__(self, arg):
        self.arg = arg

    def __str__(self):
        return f"(print {self.arg})"

    def __repr__(self):
        return self.__str__()


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


class VariableDecl(ASTNode):
    def __init__(self, var_type, identifier):
        self.var_type = var_type
        self.identifier = identifier

    def __str__(self):
        return f"({self.var_type} {self.identifier})"

    def __repr__(self):
        return self.__str__()


class Reassignment(ASTNode):
    def __init__(self, identifier, op, value):
        self.identifier = identifier
        self.token = self.op = op
        self.value = value

    def __str__(self):
        return f"({self.identifier} = {self.value})"

    def __repr__(self):
        return self.__str__()


class Variable(ASTNode):
    def __init__(self, token):
        self.token = token
        self.value = token.value

    def __str__(self):
        return f"{self.value}"

    def __repr__(self):
        return self.__str__()


class ReturnStatement(ASTNode):
    def __init__(self, return_value):
        self.value = return_value

    def __str__(self):
        return f"(return {self.value})"

    def __repr__(self):
        return self.__str__()


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


class Nop(ASTNode):
    def __str__(self):
        return ""

    def __repr__(self):
        return self.__str__()


# this is obviously bad code; I'll fix it once the prototype
# is finished and the work on the real interpreter begins
def eval_expression(expression, parser):
    if type(expression) is BinaryOp:
        return eval_bin_op(expression, parser)
    elif type(expression) in (Integer, Float, String):
        return expression.value
    elif type(expression) is Variable:
        return parser.global_scope[expression.value]["value"]
    elif type(expression) is Assignment:
        return eval_assignment(expression, parser)
    elif type(expression) is Reassignment:
        return eval_reassignment(expression, parser)
    elif type(expression) is VariableDecl:
        return eval_variable_decl(expression, parser)
    elif type(expression) is Function:
        eval_function(expression, parser)
    elif type(expression) is FunctionCall:
        return eval_function_call(expression, parser)

# evaluate function declarations
def eval_function(function: Function, parser):
    parser.functions[function.function_name.value] = function

def eval_function_call(funccall: FunctionCall, parser):
    result = None
    print(parser.functions)
    function_decl = parser.functions[funccall.func_id.value]
    return 1 # TODO

def get_var_type(variable_name, parser):
    type_str = parser.global_scope[variable_name]["type"]
    if type_str == "int":
        return int
    elif type_str == "float":
        return float
    elif type_str == "string":
        return str

def eval_variable_decl(vardec: VariableDecl, parser):
    type_string = str(vardec.var_type)
    val = None
    if type_string == "int":
        val = 0
    elif type_string == "float":
        val = 0.0
    elif type_string == "string":
        val = ""
    parser.global_scope[vardec.identifier.value] = {
        "value" : val,
        "type" : type_string
    }
    return parser.global_scope[vardec.identifier.value]

def eval_reassignment(reassgn: Reassignment, parser):
    if reassgn.identifier.value in parser.global_scope.keys():
        parser.global_scope[reassgn.identifier.value]["value"] = get_var_type(reassgn.identifier.value, parser)(eval_expression(reassgn.value, parser))
        return parser.global_scope[reassgn.identifier.value]["value"]
    else:
        raise Exception(f"Error: variable ``{reassgn.identifier.value}'' referenced before assignment")

def eval_print_statement(print_stat: PrintStatement, parser):
    if type(print_stat.arg) in (Integer, Float, String):
        print(f"{print_stat.arg.value}", end="")
    elif type(print_stat.arg) is Variable:
        print(f"{parser.global_scope[print_stat.arg.value]['value']}", end="")
    elif type(print_stat.arg) is BinaryOp:
        print(f"{eval_bin_op(print_stat.arg, parser)}", end="")
    elif type(print_stat.arg) is Assignment:
        print(f"{eval_assignment(print_stat.arg, parser)}", end="")
    elif type(print_stat.arg) is Reassignment:
        print(f"{eval_reassignment(print_stat.arg, parser)}", end="")
    elif type(print_stat.arg) is VariableDecl:
        print(f"{eval_variable_decl(print_stat.arg, parser)}", end="")

def eval_bin_op(bin_op: BinaryOp, parser):
    # Get the left operand
    left = None
    if type(bin_op.left) in (Integer, Float, String):
        left = bin_op.left.value
    elif type(bin_op.left) is Variable:
        if parser.global_scope[bin_op.left.value]["type"] == "int":
            left = int(parser.global_scope[bin_op.left.value]["value"])
        elif parser.global_scope[bin_op.left.value]["type"] == "float":
            left = float(parser.global_scope[bin_op.left.value]["value"])
        elif parser.global_scope[bin_op.left.value]["type"] == "string":
            left = str(parser.global_scope[bin_op.left.value]["value"])
    else:
        left = eval_bin_op(bin_op.left, parser)

    # Get the right operand
    right = None
    if type(bin_op.right) in (Integer, Float, String):
        right = bin_op.right.value
    elif type(bin_op.right) is Variable:
        if parser.global_scope[bin_op.right.value]["type"] == "int":
            right = int(parser.global_scope[bin_op.right.value]["value"])
        elif parser.global_scope[bin_op.right.value]["type"] == "float":
            right = float(parser.global_scope[bin_op.right.value]["value"])
        elif parser.global_scope[bin_op.right.value]["type"] == "string":
            right = str(parser.global_scope[bin_op.right.value]["value"])
    else:
        right = eval_bin_op(bin_op.right, parser)

    if type(left) is Token:
        left = left.value
    if type(right) is Token:
        right = right.value

    # Do an operation
    if bin_op.op.token_type == TokenType.MULT:
        if type(left) is str or type(left) is str:
            raise Exception("Error: operator '*' not supported for string type")
        return left * right
    elif bin_op.op.token_type == TokenType.PLUS:
        if type(left) is str or type(right) is str:
            return str(left) + str(right) # support string concatenation
        return left + right
    elif bin_op.op.token_type == TokenType.DIVIDE:
        if type(left) is str or type(left) is str:
            raise Exception("Error: operator '/' not supported for string type")
        if right == 0:
            return float("inf")
        else:
            return left / right
    elif bin_op.op.token_type == TokenType.SUBTRACT:
        if type(left) is str or type(left) is str:
            raise Exception("Error: operator '-' not supported for string type")
        return left - right

def eval_assignment(assgn: Assignment, parser):
    variable_type = None
    if str(assgn.var_type) == "int":
        variable_type = int
    elif str(assgn.var_type) == "float":
        variable_type = float
    elif str(assgn.var_type) == "string":
        variable_type = str
    parser.global_scope[assgn.identifier.value] = {
        "value" : variable_type(eval_expression(assgn.value, parser)),
        "type" : str(assgn.var_type)
    }
    return parser.global_scope[assgn.identifier.value]

def eval_program(program: Program, parser):
    for node in program.children:
        if type(node) is Assignment:
            eval_assignment(node, parser)
        elif type(node) is PrintStatement:
            eval_print_statement(node, parser)
        elif type(node) is Reassignment:
            eval_reassignment(node, parser)
        elif type(node) is VariableDecl:
            eval_variable_decl(node, parser)
        elif type(node) is Function:
            eval_function(node, parser)
        elif type(node) is FunctionCall:
            eval_function_call(node, parser)
        elif type(node) is Nop:
            pass
