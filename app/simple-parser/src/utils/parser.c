#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "parser.h"
#include "scanner.h"

OperationList opList;

typedef enum {
    PREC_NONE,
    PREC_PIPE,
    PREC_CONCAT,
    PREC_STAR,
    PREC_LITERAL,
} Precedence;

typedef void (*RuleFn)();

typedef struct {
    RuleFn prefix;
    RuleFn infix;
    RuleFn postfix;
    Precedence precedence;
} Rule;

typedef struct {
    Token previous;
    Token current;
    bool panicMode;
    bool hadError;
} Parser;

Parser parser;

static void errorAt(Token *token, const char* message){
    if(parser.panicMode) return;
    parser.panicMode = true;
    
    fprintf(stderr, "Error at token%c: %s", token->value, message);
}

static void error(const char *message){
    errorAt(&parser.previous, message);
}

static void errorAtCurrent(const char *message){
    errorAt(&parser.current, message);
}

static void advance(){
    parser.previous = parser.current;

    for(;;){
        parser.current = scanToken();

        if(parser.current.type != TOKEN_ERROR) break;
        errorAtCurrent("Unrecognized symbol.");
    }
}

static void consume(TokenType type, const char *message){
    if(parser.current.type == type){
        advance();
        return;
    }
    errorAtCurrent(message);
}

static Rule *getRule(TokenType type);
static void parsePrecedence(Precedence precedence);
static void expression();

static void appendOperation(Opcode code, char value){
    Operation op;
    op.opcode = code;
    op.value = value;
    opList.operations[opList.count++] = op;
}

static void _grouping(){
    expression();
    consume(TOKEN_RIGHT_PARENT, "Expect ']' token.");
}

static void _literal(){
    appendOperation(OP_LITERAL, parser.previous.value);
}

static void _binary(){
    TokenType type = parser.previous.type;
    Precedence precedence = getRule(type)->precedence;
    parsePrecedence((Precedence)(precedence + 1));

    switch(type){
        case TOKEN_DOT:
            appendOperation(OP_CONCAT, parser.previous.value);
            break;
        case TOKEN_PIPE:
            appendOperation(OP_PIPE, parser.previous.value);
            break;
        default:return; //unreachable
    }
}

static void _star(){
    TokenType type = parser.previous.type;
    switch(type){
        case TOKEN_STAR:
            appendOperation(OP_KLEEN, parser.previous.value);
            break;
        default:return; //unreachable
    }
}


Rule rules[] = {
    [TOKEN_LEFT_PARENT] = {_grouping, NULL, NULL, PREC_NONE},
    [TOKEN_LITERAL]     = {_literal, NULL, NULL, PREC_NONE},
    [TOKEN_STAR]        = {NULL, NULL, _star, PREC_NONE},
    [TOKEN_PIPE]        = {NULL, _binary, NULL, PREC_PIPE},
    [TOKEN_DOT]        = {NULL, _binary, NULL, PREC_CONCAT},
};

static Rule *getRule(TokenType type){
    return &rules[type];
}

static void parsePrecedence(Precedence precedence){
    advance();

    RuleFn prefixRule = getRule(parser.previous.type)->prefix;

    if(prefixRule == NULL){
        error("Expect Expression.\n");
        return;
    }
    prefixRule();

    RuleFn postfixRule = getRule(parser.current.type)->postfix;
    if(postfixRule != NULL){
        advance();
        postfixRule();
    }

    while(getRule(parser.current.type)->precedence >= precedence){
        advance();
        RuleFn infixRule = getRule(parser.previous.type)->infix;
        infixRule();
    }
}

static void expression(){
    parsePrecedence(PREC_PIPE);
}

static void init(){
    opList.count = 0;
    parser.panicMode = false;
    parser.hadError = false;
}

OperationList parse(char *regex){
    initScanner(regex);
    init();

    advance();
    expression();
    return opList;

}