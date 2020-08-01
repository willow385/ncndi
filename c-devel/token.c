#include <stdlib.h>
#include <string.h>
#include "token.h"

void free_token(struct token *t) {
    if (t == NULL) return;
    free(t->value);
    free(t);
}

void construct_token(
    struct token *dest,
    enum token_type type,
    const char *value
) {
    dest->type = type;
    size_t value_len = strlen(value) + 1;
    dest->value = malloc(value_len);
    strncpy(
        dest->value,
        value,
        value_len
    );
}

void change_token_value(
    struct token *dest,
    const char *value
) {
    free(dest->value);
    size_t value_len = strlen(value) + 1;
    dest->value = malloc(value_len);
    strncpy(
        dest->value,
        value,
        value_len
    );
}
