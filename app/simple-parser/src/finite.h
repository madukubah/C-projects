#ifndef simplex_finite_h
#define simplex_finite_h

#define MAX_STATES 100
#define MAX_SYMBOLS 128 //ascii chars

#include <stdbool.h>

#include "utils/parser.h"

typedef enum {
    NFA,
    DFA,
} FAType;

typedef struct {
    int startState;
    int stateCount;
    int accState[MAX_STATES];
    int accStateCount;
    int map[MAX_STATES][MAX_SYMBOLS][MAX_STATES];
} FiniteAuto;

typedef struct {
    int start;
    int finish;
} FiniteAutoItem;

bool isAlphaNumeric(char c);
int move(int states[], int stateCount, int c, FiniteAuto *fa);
int eClosure(int states[],int stateCount, FiniteAuto *fa);
int isMatch(const char *str, FiniteAuto *fa);
FiniteAuto *AllocateFa();
OperationList getOpList(const char *regex);
void freeFa(FiniteAuto *fa);

#endif