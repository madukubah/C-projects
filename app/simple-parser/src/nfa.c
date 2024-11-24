#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nfa.h"
#include "utils/scanner.h"

static void concat(Nfa *nfa, int c){
    printf("concat \n");
    if(nfa->stateCount + 1 > MAX_STATES){
        fprintf(stderr, "reach max states limit: %d", MAX_STATES);
        exit(1);
    }
    nfa->nfa[nfa->stateCount][(int)c][nfa->stateCount + 1] = 1;
    printf("state %d -> %d\n", nfa->stateCount, nfa->stateCount + 1);

    nfa->stateCount += 1;
    nfa->accState[nfa->accStateCount++] = nfa->stateCount;
}
static void kleen(Nfa *nfa, int c){
    printf("kleen \n");

    if(nfa->stateCount + 1 > MAX_STATES){
        fprintf(stderr, "reach max states limit: %d", MAX_STATES);
        exit(1);
    }
    int s0 = nfa->stateCount;

    nfa->nfa[nfa->stateCount][0][nfa->stateCount + 1] = 1;
    printf("state %d ->%c-> %d\n", nfa->stateCount, (char)0, nfa->stateCount + 1);
    nfa->stateCount++;

    int s1 = nfa->stateCount;
    nfa->nfa[nfa->stateCount][c][nfa->stateCount + 1] = 1;
    printf("state %d ->%c-> %d\n", nfa->stateCount, c, nfa->stateCount + 1);
    nfa->stateCount++;

    nfa->nfa[nfa->stateCount][0][s1] = 1;
    printf("state %d ->%c-> %d\n", nfa->stateCount, (char)0, s1);

    nfa->nfa[nfa->stateCount][0][nfa->stateCount + 1] = 1;
    printf("state %d ->%c-> %d\n", nfa->stateCount, (char)0, nfa->stateCount + 1);
    nfa->stateCount++;

    nfa->nfa[s0][0][nfa->stateCount] = 1;
    printf("state %d ->%c-> %d\n", s0, (char)0, nfa->stateCount);

    nfa->accState[nfa->accStateCount++] = nfa->stateCount;
}

static void _union(Nfa *nfa, int c1, int c2){
    printf("union \n");

    if(nfa->stateCount + 1 > MAX_STATES){
        fprintf(stderr, "reach max states limit: %d", MAX_STATES);
        exit(1);
    }
    int tmp1, tmp2;
    nfa->nfa[nfa->stateCount][0][nfa->stateCount + 1] = 1;
    tmp1 = nfa->stateCount + 1;
    printf("state %d ->%c-> %d\n", nfa->stateCount, (char)0, nfa->stateCount + 1);
    
    nfa->nfa[nfa->stateCount][0][nfa->stateCount + 2] = 1;
    printf("state %d ->%c-> %d\n", nfa->stateCount, (char)0, nfa->stateCount + 2);
    tmp2 = nfa->stateCount + 2;

    nfa->stateCount += 2;
    
    nfa->nfa[tmp1][(int)c1][nfa->stateCount + 1] = 1;
    printf("state %d ->%c-> %d\n", tmp1, c1, nfa->stateCount + 1);
    tmp1 = nfa->stateCount + 1;

    nfa->nfa[tmp2][(int)c2][nfa->stateCount + 2] = 1;
    printf("state %d ->%c-> %d\n", tmp2, c2, nfa->stateCount + 2);
    tmp2 = nfa->stateCount + 2;

    nfa->stateCount += 2;

    nfa->nfa[tmp1][0][nfa->stateCount + 1] = 1;
    printf("state %d ->%c-> %d\n", tmp1, (char)0, nfa->stateCount + 1);
    nfa->nfa[tmp2][0][nfa->stateCount + 1] = 1;
    printf("state %d ->%c-> %d\n", tmp2, (char)0, nfa->stateCount + 1);

    nfa->stateCount += 1;
    nfa->accState[nfa->accStateCount++] = nfa->stateCount;

}

static void buildNfa(Nfa *nfa, const char *regex){
    Scanner scanner;
    initScanner(&scanner, regex);

    while(1){
        if(isAtEnd(&scanner)) break;
        char c = advance(&scanner);
        printf("c: %c\n", c);

        if(isAphaNumeric(c) && peek(&scanner) == '*'){
            advance(&scanner);
            kleen(nfa, c);
        }
        else if(isAphaNumeric(c) && peek(&scanner) == '|' && isAphaNumeric(peekNext(&scanner))){
            advance(&scanner);
            char c2 = advance(&scanner);
            _union(nfa, c, c2);
        }
        else if(isAphaNumeric(c)){
            concat(nfa, c);
        }
        
    }
    printf("accState State: \n");
    for(int i = 0; i < nfa->accStateCount; i++){
        printf("%d, ", nfa->accState[i]);
    }
    printf("\n");
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

static int _match(const char *str, Nfa *nfa){
    printf("\nmatch\n");
    Scanner scanner;
    initScanner(&scanner, str);
    int stateCount = 0;
    int nextStates[MAX_STATES];
    
    nextStates[0] = 0;
    stateCount = eClosure(nextStates, 1, nfa);
    if(stateCount == 0){
        nextStates[0] = 0;
        stateCount = 1;
    }

    while(!isAtEnd(&scanner)){
        char c = advance(&scanner);
        printf("c: %c\n", c);

        stateCount = move(nextStates, stateCount, (int)c, nfa);
        stateCount = eClosure(nextStates, stateCount, nfa);
        if(stateCount == 0){
            stateCount = 1;
        }
    }
    for(int i = 0; i < stateCount; i++){
        printf("end states: %d\n", nextStates[i]);
        if(nextStates[i] == nfa->accState[nfa->accStateCount - 1]){
            return 1;
        }
    }
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
    buildNfa(&nfa, regex);
    if(_match(str, &nfa)){
        printf("Match!\n");
    }else{
        printf("Not Match!\n");
    }
}