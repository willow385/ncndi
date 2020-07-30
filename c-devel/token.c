#include <stdlib.h>
#include "token.h"

void free_token(struct token *t) {
    if (t == NULL) return;
    free(t->value);
    free(t);
}
