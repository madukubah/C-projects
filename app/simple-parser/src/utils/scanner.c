#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#include "scanner.h"

bool isAlpha(const char c){
    return (c >= 'a' && c <= 'z' ) || (c >= 'A' && c <= 'Z' ) || c == '_';
}

bool isDigit(const char c){
    return (c >= '0' && c <= '9' );
}

bool isAphaNumeric(const char c){
    return isAlpha(c) || isDigit(c);
}

char advance(Scanner *scanner){
    scanner->current++;
    return scanner->current[-1];
}

char peek(Scanner *scanner){
    return *scanner->current;
}

char peekNext(Scanner *scanner){
    if(isAtEnd(scanner)) return '\0';
    return scanner->current[1];
}

bool isAtEnd(Scanner *scanner){
    return *scanner->current == '\0';
}

void skipWhiteSpace(Scanner *scanner){
    for(;;){
        switch(peek(scanner)){
            case ' ':
            case '\n':
            case '\r':
            case '\t':
                advance(scanner);
                break;
            default:
                return;
        }
    }
}

void initScanner(Scanner *scanner, const char *str){
    scanner->start = str;
    scanner->current = str;
}
