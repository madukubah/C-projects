#ifndef sprsr_table_h 
#define sprsr_table_h 

#include <stdbool.h>
#include <stdint.h>

#define MAX_TOKEN 100

typedef enum {
    OBJ_STRING,
    OBJ_TOKEN,
    OBJ_RULE
} ObjType;

typedef struct Obj{
    ObjType type;
    struct Obj *next;
} Obj;

typedef struct {
    Obj obj;
    char *s;
    int length;
    uint32_t hash;
} ObjString;

typedef struct {
    Obj obj;
    ObjString *lexeme;
} ObjToken;

typedef struct ObjRule {
    Obj obj;
    ObjToken *production;
    int count;
    ObjToken *body[MAX_TOKEN];
    ObjToken *first[MAX_TOKEN];
    ObjToken *follow[MAX_TOKEN];
} ObjRule;

typedef struct {
    Obj *obj;
} Value;

typedef struct {
    ObjString *key;
    Value value;
} Entry;

typedef struct {
    int count;
    int capacity;
    Entry *entries;
} Table;

ObjString *allocateString(char *chars, int length);
ObjToken *allocateToken(ObjString *lexeme);
ObjRule *allocateRule(ObjToken *production, int count, ObjToken *body[]);
void freeObjects(Obj *obj);
void printObj(Obj *obj);
void printValue(Value value);

void initTable(Table *table);
void freeTable(Table *table);
bool tableGet(Table *table, ObjString *key, Value *value);
bool tableSet(Table *table, ObjString *key, Value value);
void printTable(Table *table);

#endif
