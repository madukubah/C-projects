#ifndef simplex_nfa_h
#define simplex_nfa_h

#define MAX_STATES 100
#define MAX_SYMBOLS 128 //ascii chars

typedef struct Nfa
{
    int nfa[MAX_STATES][MAX_SYMBOLS][MAX_STATES];
    int startState;
    int stateCount;
    int accState[MAX_STATES];
    int accStateCount;
} Nfa;

typedef struct {
    int start;
    int finish;
} NfaItem;


void nfa(const char *str, const char *regex);
void InitNfa(Nfa *nfa);

#endif