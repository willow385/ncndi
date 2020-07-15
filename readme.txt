I'm (more or less) following this course:
https://ruslanspivak.com/lsbasi-part1/

Try running `python3 mpl_interpreter.py foo.mpl`.

The language is called "MPL". "MPL" is a recursive
acronym for "MPL Programming Language". "ncndi" is
the name of this implementation of the interpreter.
I haven't decided what that stands for yet.

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

    mpl_interpreter.py: Program which ties the rest of the code
                        together into an interpreter which has
                        both an interactive mode and a batch mode.

    grammar.bnf: The grammar of MPL, expressed in a form of ABNF.

    beer.mpl: MPL implementation of "99 Bottles of Beer on the Wall".

    comparisons.mpl: MPL program showing the use of relational operators.

    conditionals.mpl: MPL program showing the use of logical operators.

    fibbonacci.mpl: MPL program that calculates and prints the first
                    twelve numbers of the Fibbonacci sequence using
                    recursion.

    hello.mpl: Hello world program in MPL.

    string_test.mpl: Program to test the limits of the MPL lexer's
                     ability to correctly parse string literals.

    type_safety.mpl: Program demonstrating type coercion in MPL.

    fizzbuzz.mpl: Implmentation of fizzBuzz in MPL using a while loop.
