# Program to print Fibonacci sequence numbers

funct fibonacci(int n) int {
    if n <= 1 {
        return n;
    } else {
        return fibonacci(n - 1) + fibonacci(n - 2);
    }
}

funct loop_thru_fib_nums(int i, int limit) {
    if i == limit {
        print fibonacci(i) + "\n";
    } else {
        print fibonacci(i) + "\n";
        loop_thru_fib_nums(i + 1, limit);
    }
}

start {
    print "The first 12 numbers of the Fibonacci sequence are:\n";
    loop_thru_fib_nums(0, 12);
} end
