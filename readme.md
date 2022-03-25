# ncndi
<sub>(This library is available under a free and permissive license)</sub>

[![Language grade: Python](https://img.shields.io/lgtm/grade/python/g/DanteFalzone0/ncndi.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/DanteFalzone0/ncndi/context:python)

The language is called "MPL". "MPL" is a recursive
acronym for "MPL Programming Language". "ncndi" is
the name of this implementation of the interpreter.
I haven't decided what that stands for yet.

I am currently in the process of writing a compiler
for MPL, which generates bytecode that can be
executed by the TMA-16 virtual machine, a RISC VM
with an instruction set and arch of my own design.
The TMA-16 was not specifically written to be a
target for MPL (and in fact poses many challenges
to writing such a compiler), but I have chosen to
target it because I know its instruction set so
well.

## Usage

To invoke the interpreter run:
```
python mpl_interpreter.py [file]
```

## File Contents

- [**ast_parser.py**](https://github.com/DanteFalzone0/ncndi/blob/master/ast_parser.py): Contains the Parser class which constructs
an abstract syntax tree out of tokens obtained
from the Lexer class and stores information
about runtime state.

- [**ast_utils.py**](https://github.com/DanteFalzone0/ncndi/blob/master/ast_utils.py): Contains classes representing various subtypes
of the ASTNode class (which in turn models nodes
of the abstract syntax tree).

- [**lexer.py**](https://github.com/DanteFalzone0/ncndi/blob/master/lexer.py): Contains the Lexer class, which scans through text
to produce tokens.

- [**token.py**](https://github.com/DanteFalzone0/ncndi/blob/master/token.py): Contains the Token class and the TokenType enum.

- [**mpl_interpreter.py**](https://github.com/DanteFalzone0/ncndi/blob/master/mpl_interpreter.py): Program which ties the rest of the code
together into an interpreter which has
both an interactive mode and a batch mode.

- [**grammar.bnf**](https://github.com/DanteFalzone0/ncndi/blob/master/grammar.bnf): The grammar of MPL, expressed in BNF.

- [**examples/**](https://github.com/DanteFalzone0/ncndi/tree/master/examples): Directory containing MPL programs.

- [**beer.mpl**](https://github.com/DanteFalzone0/ncndi/blob/master/examples/beer.mpl): MPL implementation of "99 Bottles of Beer on the Wall".

- [**comparisons.mpl**](https://github.com/DanteFalzone0/ncndi/blob/master/examples/comparisons.mpl): MPL program showing the use of relational operators.

- [**conditionals.mpl**](https://github.com/DanteFalzone0/ncndi/blob/master/examples/conditionals.mpl): MPL program showing the use of logical operators.

- [**fibonacci.mpl**](https://github.com/DanteFalzone0/ncndi/blob/master/examples/fibonacci.mpl): MPL program that calculates and prints the first
twelve numbers of the Fibonacci sequence using
recursion.

- [**hello.mpl**](https://github.com/DanteFalzone0/ncndi/blob/master/examples/hello.mpl): Hello world program in MPL.

- [**string_test.mpl**](https://github.com/DanteFalzone0/ncndi/blob/master/examples/string_test.mpl): Program to test the limits of the MPL lexer's
ability to correctly parse string literals.

- [**type_safety.mpl**](https://github.com/DanteFalzone0/ncndi/blob/master/examples/type_safety.mpl): Program demonstrating type coercion in MPL.

- [**fizzbuzz.mpl**](https://github.com/DanteFalzone0/ncndi/blob/master/examples/fizzbuzz.mpl): Implementation of FizzBuzz in MPL using a for loop.

- [**greet.mpl**](https://github.com/DanteFalzone0/ncndi/blob/master/examples/greet.mpl): Program demonstrating the built-in readln() function.
