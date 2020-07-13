#!/usr/bin/python3.8 /home/claire/ncndi/maple_interpreter.py
# Example program written in Maple

funct triple(int number) int {
    int a;
    a = 3 * number;
    return a;
}

start {

    int a;
    a = 1;
    string message = "a equals ";
    print message + a + "\n";
#    int b = triple(a);
    int b = a;
    print "3 times a equals " + b  +"\n";

} end
