#include <string.h>
#include "token.h"

#ifndef LEXER_H
#define LEXER_H

const char *reserved_words[] = {
    "start",
    "end",
    "print",
    "int",
    
};

int is_reserved_word(const char *string);



#endif // LEXER_H
