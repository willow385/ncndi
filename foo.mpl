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

    # Now to test conditional expressions.
    # Hey, I *like* using ints as conditionals.
    # It's my language and I make the rules.
    int a = 1;
    int b = 2;
    int a_gt_b = a > b;  # 1 is not greater than 2
    int a_lt_b = a < b;  # 1 is less than 2
    int a_eq_b = a == b; # 1 does not equal 2

    print a_gt_b + "\n"; # should print 0
    print a_lt_b + "\n"; # should print 1
    print a_eq_b + "\n"; # should print 0

} end
