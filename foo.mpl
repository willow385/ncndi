#!/usr/bin/python3.8 /home/claire/ncndi/maple_interpreter.py
# Example program written in Maple

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

start {

    int a;
    a = 1;
    string message = "a equals ";
    print message + a + "\n";
    int b = triple(a);
    print "3 times a equals " + b  +"\n";
    string new_message = print_message(
        "Hello, world!",
        "This string was returned from print_message()!\n"
    );
    print new_message;

} end
