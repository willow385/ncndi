#include "config.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "token.h"

void free_token(struct token *t) {
    MPL_DEBUG(fprintf(stderr, "DEBUG: Calling free_token() on token @ 0x%p.\n", (void *)t));
    if (t == NULL) return;
    free(t->value);
    free(t);
}

void construct_token(
    struct token *dest,
    enum token_type type,
    const char *value
) {
    MPL_DEBUG(fprintf(stderr, "DEBUG: Constructing token with value \"%s\" @ 0x%p.\n", value, (void *)dest));
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
    MPL_DEBUG(fprintf(stderr, "DEBUG: Changing value of token from \"%s\" to \"%s\" @ 0x%p.\n", dest->value, value, (void *)dest));
    free(dest->value);
    size_t value_len = strlen(value) + 1;
    dest->value = malloc(value_len);
    strncpy(
        dest->value,
        value,
        value_len
    );
}
