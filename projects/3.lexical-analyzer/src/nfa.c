#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "nfa.h"
#include "finite.h"

typedef struct {
    FiniteAutoItem items[1024];
    int top;
} Stack;

Stack stack;

static void initStack(){
    stack.top = 0;
}

static void push(FiniteAutoItem item){
    stack.items[stack.top++] = item;
}

static FiniteAutoItem pop(){
    return stack.items[--stack.top];
}

static void literal(FiniteAutoItem *item, FiniteAuto *fa, int c){
    int start = fa->stateCount;
    fa->map[fa->stateCount][c][fa->stateCount + 1] = 1;
    fa->stateCount++;
    int finish = fa->stateCount;

    item->start = start;
    item->finish = finish;

    fa->stateCount++;
}

static void concat(FiniteAutoItem *result, FiniteAutoItem *item1, FiniteAutoItem *item2, FiniteAuto *fa){
    fa->map[item1->finish][0][item2->start] = 1;
    result->start = item1->start;
    result->finish = item2->finish;

    fa->stateCount++;
}

static void alt(FiniteAutoItem *result, FiniteAutoItem *item1, FiniteAutoItem *item2, FiniteAuto *fa){
    int start = fa->stateCount;

    fa->map[fa->stateCount][0][item1->start] = 1;

    fa->map[fa->stateCount][0][item2->start] = 1;

    fa->map[item1->finish][0][fa->stateCount + 1] = 1;
    fa->map[item2->finish][0][fa->stateCount + 1] = 1;

    fa->stateCount++;

    int finish = fa->stateCount;
    result->start = start;
    result->finish = finish;

    fa->stateCount++;
}

static void star(FiniteAutoItem *result, FiniteAutoItem *item, FiniteAuto *fa){
    int start = fa->stateCount;

    fa->map[fa->stateCount][0][item->start] = 1;
    fa->map[item->finish][0][item->start] = 1;
    fa->map[item->finish][0][fa->stateCount + 1] = 1;
    fa->map[fa->stateCount][0][fa->stateCount + 1] = 1;

    fa->stateCount++;

    int finish = fa->stateCount;
    result->start = start;
    result->finish = finish;

    fa->stateCount++;
}

void buildNfa(FiniteAuto *fa, OperationList opList){
    initStack();

    for(int i = 0; i < opList.count; i++){
        Operation op = opList.operations[i];
        switch(op.opcode){
            case OP_LITERAL: {
                FiniteAutoItem item;
                literal(&item, fa, (int)op.value);
                push(item);
                break;
            }
            case OP_CONCAT: {
                FiniteAutoItem item2 = pop();
                FiniteAutoItem item1 = pop();
                FiniteAutoItem result;
                concat(&result, &item1, &item2, fa);
                push(result);
                break;
            }
            case OP_KLEEN:{
                FiniteAutoItem item = pop();
                FiniteAutoItem result;
                star(&result, &item, fa);
                push(result);
                break;
            }
            case OP_PIPE:{
                FiniteAutoItem item2 = pop();
                FiniteAutoItem item1 = pop();
                FiniteAutoItem result;
                alt(&result, &item1, &item2, fa);
                push(result);
                break;
            }
        }
    }
    FiniteAutoItem result = pop();
    fa->startState = result.start;
    fa->accState[fa->accStateCount++] = result.finish;

#ifdef DEBUG_MODE
    printf("accState: %d\n", fa->accState[fa->accStateCount - 1]);
#endif
}

int nfaMatch(const char *str, const char *regex){
    FiniteAuto *fa = AllocateFa();
    OperationList operationList = getOpList(regex);

    buildNfa(fa, operationList);
    
    int result = isMatch(str, fa);
    if(result){
        freeFa(fa);
        return result;
    }else{
        freeFa(fa);
        return result;
    }
}

