#!/usr/bin/env python3
from ast_utils import *
from token import *
from lexer import *
from ast_parser import *
import sys
import os

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
        print("NCNDI - the interpreter for the MPL Programming Language")
        print("Licensed under the GNU GPL, version 3 or best offer")
        print("Type in some code; when finished, ", end="")
        if os.name == "nt":
            print("press Ctrl+Z followed by Enter")
        elif os.name == "posix":
            print("press Ctrl+D")
        while True:
            try:
                text += input(">> ")
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
            print(f"Variables: {scope}")
            print(f"Functions: {funcs}")
        if "--tree" in sys.argv:
            print(syntax_tree)
    except KeyError as k:
        print(f"Error: Nonexistent variable {k} referenced")
    except Exception as e:
        print(e)

if __name__ == "__main__":
    main()
