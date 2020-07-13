#!python3 ~/ncndi/maple_interpreter.py
# Example program written in Maple

start {
    int a;
    a = 1;
    float b = 2.0;
    int sum_a_b = a + b; # my phantastic comment!

    # We can write all kinds of comments.
    # They can even be on subsequent lines.
    # Since this is an interpreted language, I've made the
    # aesthetic decision to use '#' as the comment delimeter.

    print sum_a_b; # should print 3
    sum_a_b = 2 * 7;
    print sum_a_b; # should print 14
} end
