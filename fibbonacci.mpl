# Program to print Fibbonacci sequence numbers

funct fibbonacci(int n) int {
    if n <= 1 {
        return n;
    } else {
        return fibbonacci(n - 1) + fibbonacci(n - 2);
    }
}

funct loop_thru_fib_nums(int i, int limit) {
    if i == limit {
        print fibbonacci(i) + "\n";
    } else {
        print fibbonacci(i) + "\n";
        loop_thru_fib_nums(i + 1, limit);
    }
}

start {
    print "The first 12 numbers of the Fibbonacci sequence are:\n";
    loop_thru_fib_nums(0, 12);
} end
