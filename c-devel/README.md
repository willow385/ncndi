# Notes on the C implementation of NCNDI (still under construction)

Compile everything by running `./build.sh`. This is a temporary hack; later I will write a makefile.

You can turn on debugging messages at compile time by one of the following means:

- Change the value of the `MPL_DEBUG_` macro in `config.h` to 1, or

- Pass the option `-DMPL_DEBUG_=1` to the build script; i.e. `./build.sh -DMPL_DEBUG_=1`.

To get rid of object files and executables in this directory, run `./clean.sh`.

---

# What the programs do

- `test_binary_op_0.c`: Evaluates a simple AST with just literals.

- `test_binary_op_1.c`: Evaluates an AST with a variable and a literal.

- `test_lexer.c`: Tokenizes an MPL program and prints the tokens to stdout.

- `test_calculate.c`: Constructs an AST from an MPL expression and evaluates it.

The coolest one by far is `test_calculate`. Try it out first. You don't even need to bother with
the other programs unless you're super keen on gaining a deep understanding of NCNDI's internals;
`test_calculate` is the most impressive and interesting one so far. It's basically just a simple
interactive calculator.
