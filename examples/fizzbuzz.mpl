# FizzBuzz implementation in MPL.

start {
    for int i = 0; i <= 100; i += 1 {
        if i % 15 == 0 {
            print "FizzBuzz\n";
        } else if i % 5 == 0 {
            print "Buzz\n";
        } else if i % 3 == 0 {
            print "Fizz\n";
        } else {
            print i + "\n";
        }
    }
} end
