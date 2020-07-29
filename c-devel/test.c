#include <stdio.h>
#include "token.h"

int main(void) {
    struct token example_token = {
        .type = GREATER_EQ,
        .value = ">="
    };

    printf("The GREATER_EQ token has a value of %s\n", example_token.value);
    return 0; 
}
