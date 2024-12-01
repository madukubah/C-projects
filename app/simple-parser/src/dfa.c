#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "dfa.h"
#include "nfa.h"
#include "finite.h"

int dfaStates[MAX_STATES][MAX_STATES];
int dfaStatesCount = 0;
int inputSymbols[MAX_SYMBOLS] = {0};
int inputCount = 0;

static int findDfaState(int states[], int stateCount){
    printf("findDfaState \n");
    for(int i = 0; i < stateCount; i++){
        printf("%d, ", states[i]);

    }
    printf("\n");
    printf("DfaState \n");
    for(int i = 0; i < dfaStatesCount; i++){
        for(int j = 0; j < MAX_STATES; j++){
            if(dfaStates[i][j] == 1){
                printf("%d, ", j);
            }
        }
        printf("\n");
    }
    printf("\n");
    for(int i = 0; i < dfaStatesCount; i++){
        for(int j = 0; j < stateCount; j++){
            if(dfaStates[i][states[j]] != 1){
                break;
            }
            if( j == stateCount - 1){
                return i;
            }
        }
    }
    return -1;
}
static void setAccState(FiniteAuto *dfa, FiniteAuto *fa){
    printf("setAccState \n");
    for(int i = 0; i < dfaStatesCount; i++){
        if(dfaStates[i][fa->accState[fa->accStateCount - 1]] == 1){
            dfa->accState[dfa->accStateCount++] = i;
            printf("dfa->accState %d, ", i);
            printf("fa->accState %d\n", fa->accState[fa->accStateCount - 1]);

        }
    }
}
static void subsetConstruction(FiniteAuto *dfa, FiniteAuto *fa){
    int stack[2014] = {-1};
    int stackTop = 0;

    dfaStatesCount = 0;

    int nextStates[MAX_STATES];
    int stateCount = 0;

    nextStates[0] = fa->startState;
    stateCount = eClosure(nextStates, 1, fa);
    if(stateCount == 0){
        nextStates[0] = fa->startState;
        stateCount = 1;
    }

    // dfa state A
    for(int i = 0; i < stateCount; i++){
        dfaStates[dfaStatesCount][nextStates[i]] = 1;
    }
    dfa->startState = dfaStatesCount;

    stack[stackTop++] = dfaStatesCount;
    dfaStatesCount++;

    while(stackTop != 0){
        int s = stack[--stackTop];
        printf("s %d\n", s);

        for(int i = 0; i < inputCount; i++){
            stateCount = move(nextStates, stateCount, inputSymbols[i], fa);
            stateCount = eClosure(nextStates, stateCount, fa);
            if(stateCount == 0) break;
            int find = findDfaState(nextStates, stateCount);
            printf("find %d\n", find);
            if(find == -1){
                for(int j = 0; j < stateCount; j++){
                    dfaStates[dfaStatesCount][nextStates[j]] = 1;
                }
                stack[stackTop++] = dfaStatesCount;
                find = dfaStatesCount;
                dfaStatesCount++;
                printf("find %d\n", find);
            }
            dfa->map[s][inputSymbols[i]][find] = 1;
        }
        printf("top %d\n", stackTop);
    }
    setAccState(dfa, fa);
}

static int setInputSymbols(const char *regex){
    int visited[MAX_SYMBOLS] = {0};
    inputCount = 0;

    const char *p = regex;
    while(*p != '\0'){
        if(isAlphaNumeric(*p) && visited[(int)*p] == 0){
            inputSymbols[inputCount++] = (int)*p;
            visited[(int)*p] = 1;
        }
        p++;
    }
    return inputCount;
}

void initDfa(){
    memset(dfaStates, -1, sizeof(int) * MAX_STATES * MAX_STATES);
    dfaStatesCount = 0;
    memset(inputSymbols, -1, sizeof(int) * MAX_SYMBOLS);
    inputCount = 0;
}

int dfaMatch(const char *str, const char *regex){
    FiniteAuto *fa = AllocateFa();
    OperationList operationList = getOpList(regex);
    buildNfa(fa, operationList);

    initDfa();
    setInputSymbols(regex);

    printf("setInputSymbols \n");
    for(int i = 0; i < inputCount; i++){
        printf("%c, ", (char)inputSymbols[i]);
    }
    printf("\n");


    FiniteAuto *dfa = AllocateFa();
    subsetConstruction(dfa, fa);

    if(isMatch(str, fa)){
        printf("\nMATCH!\n");
        return 1;
    }else{
        printf("\nNOT MATCH!\n");
    }
    
    freeFa(fa);
    freeFa(dfa);
    return 0;
}
