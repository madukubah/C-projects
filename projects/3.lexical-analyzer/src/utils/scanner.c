#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#include "scanner.h"

Scanner scanner;

static bool isAlpha(const char c){
    return (c >= 'a' && c <= 'z' ) || (c >= 'A' && c <= 'Z' ) || c == '_';
}

static bool isDigit(const char c){
    return (c >= '0' && c <= '9' );
}

static bool isAphaNumeric(const char c){
    return isAlpha(c) || isDigit(c);
}

static char advance(){
    scanner.current++;
    return scanner.current[-1];
}

static bool isAtEnd(){
    return *scanner.current == '\0';
}

static Token makeToken(TokenType type){
    Token token;
    token.type = type;
    token.value = *scanner.start;
    return token;
}   

Token scanToken(){
    scanner.start = scanner.current;
    if(isAtEnd()) return makeToken(TOKEN_EOF);
    
    char c = advance();

    if(isAphaNumeric(c)){
        return makeToken(TOKEN_LITERAL);
    }else{
        switch(c){
            case '(': return makeToken(TOKEN_LEFT_PARENT);
            case ')': return makeToken(TOKEN_RIGHT_PARENT);
            case '|': return makeToken(TOKEN_PIPE);
            case '*': return makeToken(TOKEN_STAR);
            case '.': return makeToken(TOKEN_DOT);
            default: 
                fprintf(stderr, "char is not recognized.\n");
                return makeToken(TOKEN_ERROR);
                break;
        }
    }
    return makeToken(TOKEN_ERROR);
}

void initScanner(const char *str){
    scanner.start = str;
    scanner.current = str;
}
