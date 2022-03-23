#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import os
import sys

from ast_parser import *
# from codegen import CodeGenerator

def main():
    text = ""
    src_file_path = ""

    # Try to find the file to run
    for i in range(len(sys.argv)):
        if i != 0 and os.path.isfile(sys.argv[i]):
            src_file_path = sys.argv[i]
            text = open(src_file_path).read()
    if src_file_path == "":
      raise Exception("No MPL source code file given")

    lexer = Lexer(text)
    parser = Parser(lexer)
    try:
        syntax_tree = parser.parse()
        scope = {}
        funcs = {"readln": "(<built-in function readln>)"}
        if "--dump" in sys.argv:
            print(f"Variables: {scope}")
            print(f"Functions: {funcs}")
        if "--tree" in sys.argv:
            print(syntax_tree)
        """ TODO implement
        code_generator = CodeGenerator(syntax_tree, scope, funcs)
        assembly = code_generator.compile()
        bytecode = assembly.assemble()
        ofpath = src_file_path.replace(".mpl", ".tmx")
        bytecode.generate_executable(path=ofpath)
        print(f"Compiled program to {ofpath}")
        """
        print("Compiler not yet implemented")
    except KeyError as k:
        print(f"Error: Nonexistent variable {k} referenced")
    except Exception as e:
        print(e)


if __name__ == "__main__":
    main()
