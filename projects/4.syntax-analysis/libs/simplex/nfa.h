#ifndef simplex_nfa_h
#define simplex_nfa_h

struct FiniteAuto;
struct OperationList;

struct FiniteAuto *buildNfafStr(const char *regex);
void buildNfa(struct FiniteAuto *fa, struct OperationList opList);
int nfaMatch(const char *str, const char *regex);

#endif
