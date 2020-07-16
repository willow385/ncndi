# program to test the parsing of string literals

start {
    string foo = "foo\"bar\"\\\n\\";
    print foo + "\n";

    # Output should look like this:
    # foo"bar"\
    # \
    #

} end
