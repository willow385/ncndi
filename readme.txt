I'm (more or less) following this course:
https://ruslanspivak.com/lsbasi-part1/

Try running `python3 maple_interpreter.py foo.mpl`.

The language is called "Maple". "ncndi" is the name of this
implementation of the interpreter. I haven't decided what
that stands for yet.

File contents:
    ast_parser.py: Contains the Parser class which constructs
                   an abstract syntax tree out of tokens obtained
                   from the Lexer class and stores information
                   about runtime state.

    ast_utils.py: Contains classes representing various subtypes
                  of the ASTNode class (which in turn models nodes
                  of the abstract syntax tree). Also contains
                  various "eval_" functions for evaluating the
                  meaning of different language constructs
                  expressed as abstract syntax tree nodes.

    lexer.py: Contains the Lexer class, which scans through text
              to produce tokens.

    token.py: Contains the Token class and the TokenType enum.

    maple_interpreter.py: Program which ties the rest of the code
                          together into an interpreter which has
                          both an interactive mode and a batch mode.

    grammar.bnf: The grammar of Maple, expressed in a form of ABNF.

    foo.mpl: A Maple program demoing many of Maple's features.

    test.mpl: A Maple program which demonstrates type coercion
              in accordance with Maple's strict type-checking.
