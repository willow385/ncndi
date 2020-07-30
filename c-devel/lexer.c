#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "token.h"
#include "lexer.h"

/* XXX No part of the code is actually using this, but
   it really,  really  seems  like it could be used in
   the future to do  some  optimizations.  Perhaps  by
   indexing into it with an enum token_type? Idk */
const char *reserved_words[] = {
    "start",
    "end",
    "print",
    "int",
    "float",
    "string",
    "funct",
    "return",
    "if",
    "else",
    "while",
    "for"
};

size_t reserved_word_count = 12;

enum token_type identifier_type(const char *identifier) {
    /* TODO find a prettier and more cache-friendly way
       to do this... this code is kind of gross and the
       assembly is probably full of jumps. */
    if (!strcmp("start", identifier)) {
        return START;
    } else if (!strcmp("end", identifier)) {
        return END;
    } else if (!strcmp("print", identifier)) {
        return PRINT;
    } else if (!strcmp("int", identifier)) {
        return TYPE_IDENTIFIER;
    } else if (!strcmp("float", identifier)) {
        return TYPE_IDENTIFIER;
    } else if (!strcmp("string", identifier)) {
        return TYPE_IDENTIFIER;
    } else if (!strcmp("funct", identifier)) {
        return FUNCTION_DECL;
    } else if (!strcmp("return", identifier)) {
        return RETURN;
    } else if (!strcmp("if", identifier)) {
        return IF;
    } else if (!strcmp("else", identifier)) {
        return ELSE;
    } else if (!strcmp("while", identifier)) {
        return WHILE;
    } else if (!strcmp("for", identifier)) {
        return FOR;
    }
    return IDENTIFIER;
}

void advance(struct lexer *lex) {
    lex->pos++;
    if (lex->pos >= lex->text_size) {
        lex->current_char = '\0';
    } else {
        lex->current_char = lex->text[lex->pos];
    }
}

void skip_whitespace(struct lexer *lex) {
    while (lex->current_char && isspace(lex->current_char)) {
        advance(lex);
    }
}

void skip_comment(struct lexer *lex) {
    if (lex->current_char == '#') {
        while (lex->current_char != '\r' && lex->current_char != '\n') {
            advance(lex);
        }
    }

    while (lex->current_char == '\r' || lex->current_char == '\n') {
        advance(lex);
    }
}

struct token *parse_number(struct lexer *lex) {
    size_t char_count = 0;
    int is_float = 0;

    // Find out how many chars we need to store the number.
    size_t i = lex->pos;
    while (lex->text[i] == '.'
           || (lex->text[i] >= '0' && lex->text[i] <= '9')
    ) {
        char_count++; i++;
        // Are we parsing a float?
        if (lex->text[i] == '.') is_float = 1;
    }

    // Give ourselves a tiny bit of extra room, just in case.
    char_count++;

    char *result_number = calloc(char_count, sizeof(char));

    // Copy the number into the allocated buffer. Wish I knew a DRYer way...
    i = 0;
    while (
        lex->text[lex->pos] == '.'
        || (lex->text[lex->pos] >= '0' && lex->text[lex->pos] <= '9')
    ) {
        result_number[i] = lex->text[lex->pos];
        ++i;
        advance(lex);
    }

    struct token *result = malloc(sizeof(struct token));
    result->type = is_float? FLOAT : INTEGER;
    result->value = result_number;
    return result;
}

struct token *parse_identifier(struct lexer *lex) {
    // Start by getting the first char, and making sure it's valid.
    char first_char;
    if (
        lex->current_char == '_'
        || (lex->current_char >= 'a' && lex->current_char <= 'z')
        || (lex->current_char >= 'A' && lex->current_char <= 'Z')
    ) {
        first_char = lex->current_char;
        advance(lex);
    } else {
        fprintf(
            stderr,
            "Error while lexing: identifiers can't start with %c\n",
            lex->current_char
        );
        return NULL;
    }

    // Next we count how much more space we're going to need.
    size_t char_count = 1;
    size_t i = lex->pos;
    while (
        lex->text[i] == '_'
        || (lex->text[i] >= 'a' && lex->text[i] <= 'z')
        || (lex->text[i] >= 'A' && lex->text[i] <= 'Z')
        || (lex->text[i] >= '0' && lex->text[i] <= '9')
    ) {
        ++i; ++char_count;
    }

    // Get the memory we need plus a little extra
    char *result_identifier = calloc(char_count + 1, sizeof(char));

    // Slurp up that juicy identifier
    result_identifier[0] = first_char;
    for (i = 1; i < char_count; ++i) {
        result_identifier[i] = lex->current_char;
        advance(lex);
    }

    struct token *result = malloc(sizeof(struct token));
    result->type = identifier_type(result_identifier);
    result->value = result_identifier;
    return result;
}

/* This condition looks butt-ugly, so I made it
   into a macro. Sue me. */
#define INSIDE_STRING_LITERAL(lex, i)         \
    lex->text[i] != '"'                       \
    || (lex->text[i] == '"'                   \
        && lex->text[i - 1] == '\\'           \
        && lex->text[i - 2] != '\\')

struct token *parse_string_literal(struct lexer *lex) {
    if (lex->current_char == '"') {
        advance(lex);
    } else {
        fprintf(stderr, "Error while lexing: String literals must begin with '\"'\n");
        return NULL;
    }

    // Count the characters
    size_t char_count = 0;
    size_t i = lex->pos;
    while (INSIDE_STRING_LITERAL(lex, i)) {
        ++char_count; ++i;
    }

    char *parsed_chars = calloc(char_count + 1, sizeof(char));

    for (i = 0; i < char_count; ++i) {
        parsed_chars[i] = lex->current_char;
        advance(lex);
    }

    char *result_string = calloc(char_count + 1, sizeof(char));

    /* Copy the characters over and handle
       escape sequences appropriately. */
    size_t j;
    for (i = 0, j = 0; i < char_count; ++i, ++j) {
        if (parsed_chars[i] == '\\') {
            switch (parsed_chars[i+1]) {
                case 'n':
                    result_string[j] = '\n';
                    break;
                case 'r':
                    result_string[j] = '\r';
                    break;
                case 't':
                    result_string[j] = '\t';
                    break;
                case '\\':
                    result_string[j] = '\\';
                    break;
                case '"':
                    result_string[j] = '"';
                    break;
            }
            ++i;
        } else {
            result_string[j] = parsed_chars[i];
        }
    }

    free(parsed_chars);
    struct token *result = malloc(sizeof(struct token));
    result->type = STRING;
    result->value = result_string;
    return result;
}

char peek(struct lexer *lex) {
    // TODO implement
    return '\0';
}

struct token *get_next_token(struct lexer *lex) {
    // TODO implement
    return NULL;
}
