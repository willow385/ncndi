#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import os
import sys

from ast_parser import *
from codegen import CodeGenerator, AssemblyCode

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
        code_generator = CodeGenerator(syntax_tree, scope, funcs)
        assembly = code_generator.compile()
        if "--dump" in sys.argv:
            print(f"Variables: {scope}")
            print(f"Functions: {funcs}")
        if "--tree" in sys.argv:
            print(syntax_tree)
        ofpath = src_file_path.replace(".mpl", ".tmx")
        assembly.assemble(of=ofpath, preserve_asm=True, gen_tmx=False)
        print(f"Compiled {src_file_path}")
    except KeyError as k:
        print(f"Error: Nonexistent variable {k} referenced")
    except Exception as e:
        print(e)


if __name__ == "__main__":
    main()
