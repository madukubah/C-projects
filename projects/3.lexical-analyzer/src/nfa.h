#ifndef simplex_nfa_h
#define simplex_nfa_h

#include "finite.h"
#include "utils/parser.h"

void buildNfa(FiniteAuto *fa, OperationList opList);
int nfaMatch(const char *str, const char *regex);

#endif