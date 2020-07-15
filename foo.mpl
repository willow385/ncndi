# Example program written in MPL

# The following shows the syntax for declaring functions.
# First the keyword "funct", then the name, then the parameters,
# then the return type (if the function returns anything),
# and then the body.
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
    # comparison operators work in MPL.
    int a = 1;
    float b = 1.0;

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

    # You can use conditional statements as expressions!
    # I personally recommend terminating them with semicolons
    # if you do this, purely for aesthetic reasons. But the
    # interpreter will handle things just fine if you don't.
    string foo = if 2 + 2 == 5 {
        return "Long live Big Brother!";
    } else {
        return "This proglang is doubleplusunpartythinkwise.";
    };

    print foo + "\n";

} end
