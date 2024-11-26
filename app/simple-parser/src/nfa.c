#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nfa.h"
#include "utils/scanner.h"
#include "utils/parser.h"

typedef struct {
    NfaItem items[1024];
    int top;
} Stack;

Stack stack;

static void initStack(){
    stack.top = 0;
}

static void push(NfaItem item){
    stack.items[stack.top++] = item;
}

static NfaItem pop(){
    return stack.items[--stack.top];
}

static bool isAlphaNumeric(char c){
    return ('a' <= c && c <= 'z') ||
            ('A' <= c && c <= 'Z') ||
            ('0' <= c && c <= '9');
}

static void addConcat(char *dst, const char *regex){
    int i, j = 0;
    char prev = '\0';
    for(i = 0;  regex[i] != '\0'; i++){
        char curr = regex[i];
        if(j >= 1){
            if(
                (isAlphaNumeric(prev) || prev == ')' || prev == '*' )
                &&
                (isAlphaNumeric(curr) || curr == '(')
            ){
                dst[j++] = '.';
            }

        }
        dst[j++] = curr;
        prev = curr;
    }
    dst[j] = '\0';
}

static void literal(NfaItem *item, Nfa *nfa, int c){
    printf("literal:\n");
    int start = nfa->stateCount;
    nfa->nfa[nfa->stateCount][c][nfa->stateCount + 1] = 1;
    printf("state %d ->%c-> %d\n", nfa->stateCount, (char)c, nfa->stateCount + 1);
    nfa->stateCount++;
    int finish = nfa->stateCount;

    item->start = start;
    item->finish = finish;

    nfa->stateCount++;
}

static void concat(NfaItem *result, NfaItem *item1, NfaItem *item2, Nfa *nfa){
    printf("concat:\n");
    nfa->nfa[item1->finish][0][item2->start] = 1;
    result->start = item1->start;
    result->finish = item2->finish;

    printf("start: %d\n", result->start);
    printf("concat %d -> %d\n", item1->finish, item2->start);
    printf("finish: %d\n", result->finish);

    nfa->stateCount++;
}

static void alt(NfaItem *result, NfaItem *item1, NfaItem *item2, Nfa *nfa){
    printf("alt:\n");
    int start = nfa->stateCount;

    nfa->nfa[nfa->stateCount][0][item1->start] = 1;
    printf("state %d ->%c-> %d\n", nfa->stateCount, (char)0, item1->start);

    nfa->nfa[nfa->stateCount][0][item2->start] = 1;
    printf("state %d ->%c-> %d\n", nfa->stateCount, (char)0, item2->start);

    nfa->nfa[item1->finish][0][nfa->stateCount + 1] = 1;
    printf("state %d ->%c-> %d\n", item1->finish, (char)0, nfa->stateCount + 1);
    nfa->nfa[item2->finish][0][nfa->stateCount + 1] = 1;
    printf("state %d ->%c-> %d\n", item2->finish, (char)0, nfa->stateCount + 1);

    nfa->stateCount++;

    int finish = nfa->stateCount;
    result->start = start;
    result->finish = finish;

    nfa->stateCount++;
}

static void star(NfaItem *result, NfaItem *item, Nfa *nfa){
    printf("star:\n");
    int start = nfa->stateCount;

    nfa->nfa[nfa->stateCount][0][item->start] = 1;
    printf("state %d ->%c-> %d\n", nfa->stateCount, (char)0, item->start);
    nfa->nfa[item->finish][0][item->start] = 1;
    printf("state %d ->%c-> %d\n", item->finish, (char)0, item->start);
    nfa->nfa[item->finish][0][nfa->stateCount + 1] = 1;
    printf("state %d ->%c-> %d\n", item->finish, (char)0, nfa->stateCount + 1);
    nfa->nfa[nfa->stateCount][0][nfa->stateCount + 1] = 1;
    printf("state %d ->%c-> %d\n", nfa->stateCount, (char)0, nfa->stateCount + 1);

    nfa->stateCount++;

    int finish = nfa->stateCount;
    result->start = start;
    result->finish = finish;

    nfa->stateCount++;
}

static void buildNfa(Nfa *nfa, OperationList opList){
    initStack();

    for(int i = 0; i < opList.count; i++){
        Operation op = opList.operations[i];
        switch(op.opcode){
            case OP_LITERAL: {
                NfaItem item;
                literal(&item, nfa, (int)op.value);
                push(item);
                break;
            }
            case OP_CONCAT: {
                NfaItem item2 = pop();
                NfaItem item1 = pop();
                NfaItem result;
                concat(&result, &item1, &item2, nfa);
                push(result);
                break;
            }
            case OP_KLEEN:{
                NfaItem item = pop();
                NfaItem result;
                star(&result, &item, nfa);
                push(result);
                break;
            }
            case OP_PIPE:{
                NfaItem item2 = pop();
                NfaItem item1 = pop();
                NfaItem result;
                alt(&result, &item1, &item2, nfa);
                push(result);
                break;
            }
        }
    }
    NfaItem result = pop();
    nfa->startState = result.start;
    nfa->accState[nfa->accStateCount++] = result.finish;
    printf("accState: %d\n", nfa->accState[nfa->accStateCount - 1]);

}



static int move(int states[MAX_STATES], int stateCount, int c, Nfa *nfa){
    printf("move({");
    for(int i = 0; i < stateCount; i++){
        printf("%d, ", states[i]);
    }
    printf("}, %d)\n", c);

    int dstStates[MAX_STATES];
    int count = 0;
    for(int i = 0; i < stateCount; i++){
        for(int j = 0; j < MAX_STATES; j++){
            if(nfa->nfa[states[i]][c][j] == 1){
                dstStates[count++] = j;
            }
        }
    }
    for(int i = 0; i < count; i++){
        states[i] = dstStates[i];
        printf("%d, ", states[i]);

    }
    printf("\n");
    return count;
}

static int eClosure(int states[MAX_STATES], int stateCount, Nfa *nfa){
    printf("eClosure: \n");
    for(int i = 0; i < stateCount; i++){
        printf("%d, ", states[i]);

    }
    printf("\n");

    int closure[MAX_STATES];
    int closureCount = 0;
    int visited[MAX_STATES] = {0};
    int stack[MAX_STATES];
    int top = 0;

    for(int i=0; i < stateCount; i++){
        int s = states[i];
        if(!visited[s]){
            stack[top++] = s;
            visited[s] = 1;

            closure[closureCount++] = s;
        }
    }
    while(top != 0){
        int s = stack[--top];

        for(int i = 0; i < MAX_STATES; i++){
            if(nfa->nfa[s][0][i] == 1 && !visited[i]){
                stack[top++] = i;
                visited[i] = 1;

                closure[closureCount++] = i;
            }
        }
    }

    printf("result: \n");
    for(int i = 0; i < closureCount; i++){
        states[i] = closure[i];
        printf("%d, ", states[i]);

    }
    printf("\n");
    return closureCount;
}

static int match(const char *str, Nfa *nfa){
    const char *p = str;

    int stateCount = 0;
    int nextStates[MAX_STATES];
    nextStates[0] = nfa->startState;

    stateCount = eClosure(nextStates, 1, nfa);
    if(stateCount == 0){
        nextStates[0] = nfa->startState;
        stateCount = 1;
    }

    while(*p != '\0'){
        char c = *p;
        printf("c: %c\n", c);
        stateCount = move(nextStates, stateCount, (int)c, nfa);
        stateCount = eClosure(nextStates, stateCount, nfa);
        if(stateCount == 0){
           stateCount = 1; 
        }
        p++;
    }


    printf("end states: ");
    for(int i = 0; i < stateCount; i++){
        printf("%d, ", nextStates[i]);
        if(nextStates[i] == nfa->accState[nfa->accStateCount - 1]){
            return 1;
        }
    }
    printf("\n");

    return 0;

}

void InitNfa(Nfa *nfa){
    memset(nfa->nfa, -1, sizeof(int) * MAX_STATES * MAX_SYMBOLS * MAX_STATES);
    nfa->startState = 0;
    nfa->stateCount = 0;
    memset(nfa->accState, -1, sizeof(int) * MAX_STATES);
    nfa->accStateCount = 0;
}

void nfa(const char *str, const char *regex){
    printf("str len: %zu\n", strlen(str));
    printf("regex len: %zu\n", strlen(regex));

    Nfa nfa;
    InitNfa(&nfa);

    char augmentedRegex[512];
    addConcat(augmentedRegex, regex);
    printf("augmentedRegex: %s\n", augmentedRegex);

    OperationList operationList = parse(augmentedRegex);
    printf("\n\n");

    for(int i = 0; i < operationList.count; i++){
        Operation op = operationList.operations[i];
        switch(op.opcode){
            case OP_LITERAL:
                printf("Literal: %c\n", op.value);
                break;
            case OP_CONCAT:
                printf("CONCAT\n");
                break;
            case OP_KLEEN:
                printf("KLEEN\n");
                break;
            case OP_PIPE:
                printf("PIPE\n");
                break;
        }
    }

    printf("\n\n");

    buildNfa(&nfa, operationList);

    printf("\n\n");

    if(match(str, &nfa)){
        printf("\nMATCH!\n");
    }else{
        printf("\nNOT MATCH!\n");
    }
}