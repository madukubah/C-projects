#ifndef simplex_scanner_h
#define simplex_scanner_h

#include <stdbool.h>

typedef enum {
    TOKEN_LITERAL, // a , b, 1, 2
    TOKEN_STAR, // *
    TOKEN_PIPE, // |
    TOKEN_LEFT_PARENT, // (
    TOKEN_RIGHT_PARENT, // )
    TOKEN_DOT, // \.
    TOKEN_EOF,
    TOKEN_ERROR,
} TokenType;

typedef struct {
    TokenType type;
    char value;
} Token;

typedef struct {
    const char *start;
    const char *current;
} Scanner;

void initScanner(const char *str);
Token scanToken();

#endif