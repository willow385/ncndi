# FizzBuzz implementation in MPL.

funct is_divisible(int n, int divisor) int {
    int quotient_i   = n / divisor;
    float quotient_f = n / divisor;
    return quotient_i == quotient_f;
}

start {
    for int i = 0; i <= 100; i = i + 1 {
        if is_divisible(i, 15) {
            print "FizzBuzz\n";
        } else if is_divisible(i, 5) {
            print "Buzz\n";
        } else if is_divisible(i, 3) {
            print "Fizz\n";
        } else {
            print i + "\n";
        }
    }
} end
