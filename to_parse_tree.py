from ast_utils import *
from token import *
from lexer import *
from ast_parser import *

def main():
    text = input("Enter an expression >> ")
    lexer = Lexer(text)
    parser = Parser(lexer)
    print(str(parser.parse()))

if __name__ == "__main__":
    main()
