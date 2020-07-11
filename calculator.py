#!/usr/bin/env python3
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

        try:
            lexer = Lexer(text)
            parser = Parser(lexer)
            print(eval_bin_op(parser.parse()))
        except Exception as e:
            print(e)
            continue


if __name__ == "__main__":
    main()
