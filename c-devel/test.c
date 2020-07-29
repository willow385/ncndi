#include <stdio.h>
#include "token.h"

int main(void) {
    char *program_code = "start { print \"Hello, world!\\n\" } end\n";
    printf("The program that we want to lex is: %s\n", program_code);
    return 0;
}
