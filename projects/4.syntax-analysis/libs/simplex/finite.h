#ifndef simplex_finite_h
#define simplex_finite_h

//#define MAX_STATES 100
//#define MAX_SYMBOLS 128 //ascii chars

//#include <stdbool.h>

//typedef enum {
    //NFA,
    //DFA,
//} FAType;

//typedef struct FiniteAuto {
    //int startState;
    //int stateCount;
    //int accState[MAX_STATES];
    //int accStateCount;
    //int map[MAX_STATES][MAX_SYMBOLS][MAX_STATES];
//} FiniteAuto;

struct FiniteAuto;

//typedef struct {
    //int start;
    //int finish;
//} FiniteAutoItem;

//bool isAlphaNumeric(char c);
//int move(int states[], int stateCount, int c, FiniteAuto *fa);
//int eClosure(int states[],int stateCount, FiniteAuto *fa);
int isMatch(const char *str, struct FiniteAuto *fa);
//struct FiniteAuto *AllocateFa();
void freeFa(struct FiniteAuto *fa);

#endif
