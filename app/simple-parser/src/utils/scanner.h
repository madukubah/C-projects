#ifndef simplex_scanner_h
#define simplex_scanner_h

#include <stdbool.h>

typedef struct {
    const char *start;
    const char *current;
} Scanner;
    

void initScanner(Scanner *scanner, const char *str);
char advance(Scanner *scanner);
char peek(Scanner *scanner);
char peekNext(Scanner *scanner);
bool isAtEnd(Scanner *scanner);
void skipWhiteSpace(Scanner *scanner);
bool isAphaNumeric(const char c);
bool isDigit(const char c);
bool isAlpha(const char c);

#endif