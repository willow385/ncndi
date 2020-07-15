#!/usr/bin/python3.8 /home/claire/ncndi/mpl_interpreter.py
# Example program written in MPL

# The following shows the syntax for declaring functions.
funct triple(int number) int {
    int a;
    a = 3 * number;
    return a;
}

# Functions can have side-effects and are strictly typed.
# They can ONLY return values of their specified return type.
funct print_message(string message, string return_val) string {
    print "\"" + message + "\" was passed to print_message()!\n";
    return return_val;
}

# Functions with no return type are implicitly typed as "void"
# under the hood. They can't return anything.
funct print_other_message() {
    print "print_other_message() was called.\n";
}

start {

    string new_message = print_message(
        "Hello, world!",
        "This string was returned from print_message()!\n"
    );

    print new_message;
    print_other_message();

    # Now to test conditionals.
    # Change these to whatever you like to see how the
    # comparison operator works in MPL.
    int a = 1;
    float b = 1.0;

    if 1 == 1 {
        int c = 69420;
    }

    if b > a {
        print "b is greater than a!\n";
    } else if b == a {
        print "b equals a!\n";
    } else if b < a {
        print "b is less than a!\n";
    } else {
        # Control flow will come here if you try to
        # compare a string with a number.
        print "b and a are not comparable!\n";
    }


} end
