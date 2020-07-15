# Program to demonstrate the logical operators

start {

    int true  = (1 == 1);
    int false = (1 == 0);

    # AND operator
    if true && true {
        print "AND operator works!\n";
    }
    if true && false {
        print "Actually, wait, no it doesn't...\n";
    }
    if false && false {
        print "This should DEFINITELY not happen!\n";
    }

    # OR operator
    if true || true {
        print "So far the OR operator appears to work...\n";
    }
    if true || false {
        print "OR operator still working.\n";
    }
    if false || false {
        print "Nope, OR operator is borken.\n";
    }

    # XOR operator
    if true <> true {
        print "The XOR operator is not working correctly.\n";
    }
    if true <> false {
        print "The XOR operator passed this test!\n";
    }
    if false <> false {
        print "This shouldn't happen!\n";
    }

    # NOT operator
    if !true {
        print "The NOT operator is NOT working right!\n";
    }
    if !false {
        print "The NOT operator is working just fine.\n";
    }

} end
