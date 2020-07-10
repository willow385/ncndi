from ast_utils import *
from token import *
from lexer import *
from ast_parser import *

def main():
    while True:
        try:
            text = input("Dante's Calculator Repl > ")
        except EOFError:
            break
        if not text:
            continue

        lexer = Lexer(text)
        parser = Parser(lexer)
        print(eval_bin_op(parser.parse()))


if __name__ == "__main__":
    main()
