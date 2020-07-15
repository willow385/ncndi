funct foo() int {
    int i = 0;
    while i < 10 {
        i = i + 1;
        if i == 8 {
            print i + "\n";
            return 69420;
        }
    }
}

start {
    print foo() + "\n";
} end
