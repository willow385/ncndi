# This program checks to make sure the type system is
# working properly. It should print "3" followed by "3.0".

start {

    int foo = 3.0;   # coerced to int
    float bar = foo; # coerced to float

    print foo; # 3
    print bar; # 3.0

    # TODO: implement string type

} end