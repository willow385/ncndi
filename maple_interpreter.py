from ast_utils import *
from token import *
from lexer import *
from ast_parser import *
import sys

def main():
    text = ""
    if len(sys.argv) > 1:
        text = open(sys.argv[1]).read()
    else:
        while True:
            try:
                text += input("Enter some code, send EOF when done >> ")
            except EOFError:
                break

    lexer = Lexer(text)
    parser = Parser(lexer)
    eval_program(parser.parse(), parser)

if __name__ == "__main__":
    main()
