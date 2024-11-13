#ifndef simple_scanner_h
#define simple_scanner_h

#include <stdbool.h>

typedef struct {
    char *start;
    char *current;
} Scanner;
    

void initScanner(Scanner *scanner, char *str);
char advance(Scanner *scanner);
char peek(Scanner *scanner);
bool isAtEnd(Scanner *scanner);
void skipWhiteSpace(Scanner *scanner);
bool isAphaNumeric(const char c);
bool isDigit(const char c);
bool isAlpha(const char c);
char *readLine(Scanner *scanner);
#endif