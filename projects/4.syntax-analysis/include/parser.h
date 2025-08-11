#ifndef sprsr_parser_h
#define sprsr_parser_h

#include "table.h"
#include "simplex/nfa.h"

typedef struct {
    Table tokenTable;
    Table ruleTable;
    ObjToken *start;
    struct FiniteAuto *fa;
    Obj *objs;
} Parser;

extern Parser parser;

void initParser(const char *source);
void parse(const char *source);

#endif
