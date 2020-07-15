# Implementation of "99 Bottles of Beer on the Wall" in MPL, using recursive
# function calls.
funct beer(int n) {
    if n > 0 && n != 1 {

        print n + " bottles of beer on the wall, " + n + " bottles of beer!\n";
        print "Take one down, pass it around,\n";

        if (n - 1) > 1 {
            print (n - 1) + " more bottles of beer on the wall.\n";
        } else {
            print "One more bottle of beer on the wall.\n";
        }

        beer(n - 1);

    } else if n == 1 {

        print "One last bottle of beer on the wall, one last bottle of beer!\n";
        print "Take it down, pass it around,\n";
        beer(n - 1);

    } else {
        print "No more bottles of beer on the wall.\n";
    }
}

start {
    beer(99);
} end
