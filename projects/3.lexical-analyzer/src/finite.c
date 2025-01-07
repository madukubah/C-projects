#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "common.h"
#ifdef DEBUG_MODE
#include "debug.h"
#endif

#include "finite.h"
//#include "parser.h"

/*bool isAlphaNumeric(char c){*/
    /*return ('a' <= c && c <= 'z') ||*/
            /*('A' <= c && c <= 'Z') ||*/
            /*('0' <= c && c <= '9');*/
/*}*/

int move(int states[], int stateCount, int c, FiniteAuto *fa){
#ifdef DEBUG_MODE
    printf("move %c\n", c);
    for(int i = 0; i < stateCount; i++){
        printf("%d, ", states[i]);

    }
    printf("\n");
#endif
    
    int dstStates[MAX_STATES];
    int count = 0;
    for(int i = 0; i < stateCount; i++){
        for(int j = 0; j < MAX_STATES; j++){
            if(fa->map[states[i]][c][j] == 1){
                dstStates[count++] = j;
            }
        }
    }
    for(int i = 0; i < count; i++){
        states[i] = dstStates[i];
    }

#ifdef DEBUG_MODE
    for(int i = 0; i < count; i++){
        printf("%d, ", states[i]);

    }
    printf("\n");
#endif
    return count;
}

int eClosure(int states[],int stateCount, FiniteAuto *fa){
#ifdef DEBUG_MODE
    printf("eClosure \n");
    for(int i = 0; i < stateCount; i++){
        printf("%d, ", states[i]);
    }
#endif
    

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
            if(fa->map[s][0][i] == 1 && !visited[i]){
                stack[top++] = i;
                visited[i] = 1;

                closure[closureCount++] = i;
            }
        }
    }

    for(int i = 0; i < closureCount; i++){
        states[i] = closure[i];

    }

#ifdef DEBUG_MODE
    printf("\n");
    for(int i = 0; i < closureCount; i++){
        printf("%d, ", states[i]);
    }
    printf("\n");
#endif

    return closureCount;
}

int isMatch(const char *str, FiniteAuto *fa){
#ifdef DEBUG_MODE
    printf("isMatch: %s\n", str);
#endif
    const char *p = str;
    int length = 0;
    int prevStates[MAX_STATES];
    int prevStateCount = 0;
    
    int nextStates[MAX_STATES];
    int stateCount = 0;
    nextStates[0] = fa->startState;

    stateCount = eClosure(nextStates, 1, fa);
    if(stateCount == 0){
        nextStates[0] = fa->startState;
        stateCount = 1;
    }
    memcpy(prevStates, nextStates, sizeof(int) * stateCount);

    while(*p != '\0'){
        char c = *p;
        stateCount = move(nextStates, stateCount, (int)c, fa);
        stateCount = eClosure(nextStates, stateCount, fa);
        if(stateCount == 0) break;
        
        memcpy(prevStates, nextStates, sizeof(int) * stateCount);
        prevStateCount = stateCount;
        p++;
        length++;
    }


    // printf("Check State \n");
    for(int i = 0; i < prevStateCount; i++){
        // printf("%d, %d \n", nextStates[i], fa->accState[fa->accStateCount - 1]);
        if(prevStates[i] == fa->accState[fa->accStateCount - 1]){
            return length;
        }
    }

    return 0;
}

struct FiniteAuto *AllocateFa(){
    FiniteAuto *fa = (FiniteAuto *)malloc(sizeof(FiniteAuto));
    memset(fa->map, -1, sizeof(int) * MAX_STATES * MAX_SYMBOLS * MAX_STATES);
    fa->startState = 0;
    fa->stateCount = 0;
    memset(fa->accState, -1, sizeof(int) * MAX_STATES);
    fa->accStateCount = 0;
    return fa;
}

void freeFa(FiniteAuto *fa){
    free(fa);
}
