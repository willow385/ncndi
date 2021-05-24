#!/bin/sh
# TODO write a makefile. Build scripts like this are temporary hacks.
echo "Building lexer..."
gcc -c -g lexer.c $@
echo "Building token..."
gcc -c -g token.c $@
echo "Building binary_op..."
gcc -c -g binary_op.c $@
echo "Building mpl_object..."
gcc -c -g mpl_object.c $@
echo "Building mpl_variable..."
gcc -c -g mpl_variable.c $@
echo "Building key_pair..."
gcc -c -g key_pair.c $@
echo "Building mpl_program_block..."
gcc -c -g mpl_program_block.c $@
echo "Building parser..."
gcc -c -g parser.c $@
echo "Building lexer test..."

gcc -g test_lexer.c \
    binary_op.o \
    key_pair.o \
    token.o \
    parser.o \
    mpl_program_block.o \
    lexer.o \
    mpl_object.o \
    mpl_variable.o \
    -o test_lexer $@

echo "Building binary_op test 0..."

gcc -g test_binary_op_0.c \
    binary_op.o \
    key_pair.o \
    token.o \
    parser.o \
    mpl_program_block.o \
    lexer.o \
    mpl_object.o \
    mpl_variable.o \
    -o test_binary_op_0 $@

echo "Building binary_op test 1..."

gcc -g test_binary_op_1.c \
    binary_op.o \
    key_pair.o \
    token.o \
    parser.o \
    mpl_program_block.o \
    lexer.o \
    mpl_object.o \
    mpl_variable.o \
    -o test_binary_op_1 $@

echo "Building calculation test..."

gcc -g test_calculate.c \
    binary_op.o \
    key_pair.o \
    token.o \
    parser.o \
    mpl_program_block.o \
    lexer.o \
    mpl_object.o \
    mpl_variable.o \
    -o test_calculate $@

echo "Building mpl_program_block test..."

gcc -g test_mpl_program_block.c \
    binary_op.o \
    key_pair.o \
    token.o \
    parser.o \
    mpl_program_block.o \
    lexer.o \
    mpl_object.o \
    mpl_variable.o \
    -o test_mpl_program_block $@

echo "Done building!"
