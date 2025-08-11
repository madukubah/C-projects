#include <stdio.h>
#include <stdlib.h>
#include "calc.h"

#define MAXOP 100

int main(){
    int type;
    double op2;
    char s[MAXOP];
    
    while((type = getop(s)) != EOF){
        printf("text: %s\n", s);
        switch(type){
            case NUMBER:
                push(atof(s));
                break;
            case '+':
                push(pop() + pop());
                break;
            case '\n':
                printf("\t%.8g \n", pop());
                break;
            default:
                printf("error: unknown command %s\n", s);
                break;
        }
    }

    return 0;
}
