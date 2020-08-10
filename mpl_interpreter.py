#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import os
import sys

from ast_parser import *


def main():
    text = ""
    interactive_mode = False

    # Try to find the file to run
    for i in range(len(sys.argv)):
        if i != 0 and os.path.isfile(sys.argv[i]):
            text = open(sys.argv[i]).read()
    if text == "":
        interactive_mode = True

    if interactive_mode:
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
                print("")
                break

    lexer = Lexer(text)
    parser = Parser(lexer)
    try:
        syntax_tree = parser.parse()
        scope = {}
        funcs = {"readln": "(<built-in function readln>)"}
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
