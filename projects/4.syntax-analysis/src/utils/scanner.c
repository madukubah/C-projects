#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "simplex/finite.h"

#include "scanner.h"
#include "table.h"
#include "parser.h"

typedef enum {
    NONE,
    DECLARATION,
    RULE
} SectionType;

typedef struct {
    const char *start;
    const char *current;
} Scanner;

static Scanner scanner;
static SectionType section;
void (*buildFa)(); 
static char advance(){
    scanner.current++;
    return scanner.current[-1];
}

static bool isAtEnd(){
    return *scanner.current == '\0';
}

static char peekNext(){
    if(isAtEnd()) return '\0';
    return scanner.current[1];
}

static char peek(){
    return *scanner.current;
}

static void skipWhiteSpace(){
    for(;;){
        char c = peek();
        switch(c){
            case ' ':
            case '\r':
            case '\n':
            case '\t':
                advance();
                break;
            default: return;
        }
    }
}

static bool isAlpha(char c){
    return ('a' <= c && c <= 'z') ||
            ('A' <= c && c <= 'Z') ||
            c == '_' || c == '`';
}

static bool isNumber(char c){
    return '0' <= c && c <= '9';
}

static bool isAlphaNumeric(char c){
    return isAlpha(c) || isNumber(c);
}

static ObjString *_string(){
    skipWhiteSpace();
    scanner.start = scanner.current;
        
    while(!isAtEnd() && isAlphaNumeric(peek())){
        advance();
    }
    
    int length = scanner.current - scanner.start;
    if(length == 0) return NULL;

    char *s = malloc(length + 1);
    memcpy(s, scanner.start, length);
    s[length] = '\0';

    ObjString *key = allocateString(s, length);
    return key;
}

static ObjToken *token(){
    ObjString *key = _string();
    if(key == NULL) return NULL;
    ObjToken *token = allocateToken(key);

    tableSet(&parser.tokenTable, key, (Value){(Obj *)token});
    return token;
}

static void tokens(){
    for(;;){
        /*skipWhiteSpace();*/
        /*scanner.start = scanner.current;*/
        
        if(isAtEnd() || peek() == '%') return;
        token();
        /*char c = advance();*/
        /*if(isAlpha(c)){*/
            /*token();*/
        /*}*/
    }
}

static void declaration(){
    for(;;){
        skipWhiteSpace();
        scanner.start = scanner.current;
        if(isAtEnd()) return;
        if(peek() == '%' && peekNext() == '%') return;

        char c = advance();
        switch(c){
            case '%':{
               while(!isAtEnd() && isAlpha(peek())) advance();
               int len = scanner.current - scanner.start;
               if(len == 6 && memcmp(scanner.start, "%token", 6) == 0){
                    printf("decl token\n");
                    tokens();
                    /*buildFa();*/
               }
               else if(len == 6 && memcmp(scanner.start, "%start", 6) == 0){
                    if(parser.fa == NULL) continue;
                    /*TODO:*/
                    /*assign start token(header production) to parser obj*/
                    /*parser.start = */
               }
            }
            break;
        }
    }
}

static void readLine(){
    scanner.start = scanner.current;
    while(!isAtEnd() && peek() != '\n'){
        advance();
    }

}

static ObjRule *rule(const char *str, int length){
    ObjToken *production;
    ObjToken *body[MAX_TOKEN];
    int i = 0;
    int count = 0;
    while(i < length){
        if(parser.fa == NULL) return NULL;
        int matchLength = isMatch(str + i, parser.fa);
        if(matchLength > 0){
            char *s = (char *)malloc(matchLength + 1);
            memcpy(s, str + i, matchLength);
            s[matchLength] = '\0';
            ObjString *objString = allocateString(s, matchLength);
            Value value;
	        if(tableGet(&parser.tokenTable, objString, &value)){
	    	    if(count == 0){
            	    /*create production*/
                    production = (ObjToken *) value.obj; 
            	}else{
                    /*create body*/
                	body[count - 1] = (ObjToken *) value.obj;
            	}
            	i += matchLength;
            	count++;
	        }
        }else{
            i++;
        }
    }
    if(count > 1){
        ObjRule *rule = allocateRule(production, count - 1, body);
        tableSet(&parser.ruleTable, production->lexeme, (Value){(Obj *)rule});
        return rule;
    }
    return NULL;
}

static void rules(){
/*TODO:*/
    /*readline*/
    /*matchNfa to validate an allocate rule*/
        /*rule : token : token token**/
        /*store rule in rule table, production token as its key*/
        /*any token that appears in ruleTable keys and has 1 or more tokens is nonterminal else its terminal*/

    /*store rule in parser.ruleTable*/
    for(;;){
        skipWhiteSpace();
        readLine();
        rule(scanner.start, scanner.current - scanner.start);
	    if(isAtEnd()) return;
        /*printf("line: \n");*/
        /*write(1, scanner.start, scanner.current - scanner.start);*/
        /*printf("\n");*/
    }
}

void initScanner(const char *source){
    scanner.start = source; 
    scanner.current = source;
    
    section = NONE;
}

void scan(void (*buildFa)()){
    buildFa = buildFa;
    for(;;){
        skipWhiteSpace();
        scanner.start = scanner.current;

        if(isAtEnd()) break;
        
        char c = advance();

        if(isAlpha(c)){
            if(section == DECLARATION){
                token();
            }
        };

        switch(c){
            case '%':
                if(peek() == '%'){
                    advance();
                    if(section == NONE){
                        section = DECLARATION;
                        printf("section = %d\n", section);
                        declaration();
                        buildFa();
                    } 
                    else if(section == DECLARATION){
                        section = RULE;
                        printf("section = %d\n", section);
                        rules();
                    } 
                }
                break;
        }
    }
}
