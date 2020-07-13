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


class Nop(ASTNode):
    def __str__(self):
        return ""

    def __repr__(self):
        return self.__str__()


def eval_bin_op(bin_op: BinaryOp):
    # Get the left operand
    left = int()
    if type(bin_op.left) is Integer:
        left = bin_op.left.value
    else:
        left = eval_bin_op(bin_op.left)

    # Get the right operand
    right = int()
    if type(bin_op.right) is Integer:
        right = bin_op.right.value
    else:
        right = eval_bin_op(bin_op.right)

    # Do an operation
    if bin_op.op.token_type == TokenType.MULT:
        return left * right
    elif bin_op.op.token_type == TokenType.PLUS:
        return left + right
    elif bin_op.op.token_type == TokenType.DIVIDE:
        if right == 0:
            return float("inf")
        else:
            return left // right
    elif bin_op.op.token_type == TokenType.SUBTRACT:
        return left - right

def eval_program(program: Program, parser):
    # TODO
    pass
