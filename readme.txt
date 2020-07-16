I'm (more or less) following this course:
https://ruslanspivak.com/lsbasi-part1/

The language is called "MPL". "MPL" is a recursive
acronym for "MPL Programming Language". "ncndi" is
the name of this implementation of the interpreter.
I haven't decided what that stands for yet.

To invoke the interpreter run `python mpl_interpreter.py [file]`.

File contents:
    ast_parser.py: Contains the Parser class which constructs
                   an abstract syntax tree out of tokens obtained
                   from the Lexer class and stores information
                   about runtime state.

    ast_utils.py: Contains classes representing various subtypes
                  of the ASTNode class (which in turn models nodes
                  of the abstract syntax tree).

    lexer.py: Contains the Lexer class, which scans through text
              to produce tokens.

    token.py: Contains the Token class and the TokenType enum.

    mpl_interpreter.py: Program which ties the rest of the code
                        together into an interpreter which has
                        both an interactive mode and a batch mode.

    grammar.bnf: The grammar of MPL, expressed in a form of ABNF.

    examples/: Directory containing MPL programs.

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

        fizzbuzz.mpl: Implmentation of fizzBuzz in MPL using a for loop.
