#!/usr/bin/env python3
from ast_utils import *
from token import *
from lexer import *
from ast_parser import *
import sys

def main():
    text = ""

    if len(sys.argv) > 1:
        # best code ever written /s
        # obviously this is just a prototype lmao
        try:
            text = open(sys.argv[1]).read()
        except FileNotFoundError:
            text = open(sys.argv[2]).read()
        except FileNotFoundError:
            text = open(sys.argv[3]).read()
    else:
        while True:
            try:
                text += input("Enter some code, send EOF when done >> ")
            except EOFError:
                break

    lexer = Lexer(text)
    parser = Parser(lexer)
    try:
        syntax_tree = parser.parse()
        scope = {}
        funcs = {}
        syntax_tree.eval(scope, funcs)
        if "--dump" in sys.argv:
            print(parser.global_scope)
        if "--tree" in sys.argv:
            print(syntax_tree)
    except Exception as e:
        print(e)

if __name__ == "__main__":
    main()
