#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "table.h"
#include "parser.h"

#define ALLOCATE_OBJ(type, objType) \
    (type *)allocateObj(objType, sizeof(type))

static Obj *allocateObj(ObjType type, size_t size){
    Obj *obj = (Obj *) malloc(size);
    obj->type = type;

    obj->next = parser.objs;
    parser.objs = obj;

    return obj;
}

static uint32_t hashString(const char *chars, int length){
    uint32_t hash = 2166136261u;
    for(int i = 0; i < length; i++){
        hash ^= (uint32_t)chars[i];
        hash *= 16777619;
    }

    return hash;
}

ObjString *allocateString(char *chars, int length){
    ObjString *objString = ALLOCATE_OBJ(ObjString, OBJ_STRING);
    objString->s = chars;
    objString->length = length;
    objString->hash = hashString(chars, length);

    return objString;
}

ObjToken *allocateToken(ObjString *lexeme){
    ObjToken *objToken = ALLOCATE_OBJ(ObjToken, OBJ_TOKEN);
    objToken->lexeme = lexeme;
    return objToken;
}

ObjRule *allocateRule(ObjToken *production, int count, ObjToken *body[]){
    ObjRule *rule = ALLOCATE_OBJ(ObjRule, OBJ_RULE);
    rule->production = production;

    for(int i = 0; i < count; i++){
        rule->body[i] = body[i];
    }
    rule->count = count;
    
    return rule;
}

void printObj(Obj *obj){
    switch(obj->type){
        case OBJ_STRING:{
            ObjString *objString = (ObjString *)obj;
            printf("STRING %s \n", objString->s);
            break;
        }
        case OBJ_TOKEN:{
            ObjToken *objToken = (ObjToken *)obj;
            printf("TOKEN %s \n", objToken->lexeme->s);
            break;
        }
        case OBJ_RULE:{
            ObjRule *objRule = (ObjRule *)obj;
            printf("RULE %s : ", objRule->production->lexeme->s);
            for(int i = 0; i < objRule->count; i++){
                printf("%s ", objRule->body[i]->lexeme->s);
            }
            printf("\n");
            break;
        }
    }
}

void printValue(Value value){
    printObj((Obj *)value.obj);
}
static void freeObject(Obj *obj){
    printf("free Object: ");
    printObj(obj);
    switch(obj->type){
        case OBJ_STRING:{
            ObjString *objString = (ObjString *)obj;
            free(objString->s);
            free(objString);
            break;
        }        
        case OBJ_TOKEN:{
            free((ObjToken *)obj);
            break;
        }        
        case OBJ_RULE:{
            free((ObjRule *)obj);
            break;
        }        
    }
}

void freeObjects(Obj *obj){
    Obj *p = obj;
    while(p != NULL){
        Obj *next = p->next;
        freeObject(p);
        p = next;
    }
}

// develop openaddress hash table

static Entry *findEntry(Entry *entries, int capacity, ObjString *key){
    uint32_t index = key->hash % capacity;
    for(;;){
        Entry *entry = &entries[index];
        if(entry->key == NULL){
            return entry;
        }
        else if(entry->key->length == key->length &&
                entry->key->hash == key->hash &&
                memcmp(entry->key->s, key->s, (size_t)key->length) == 0
                ){
            return entry;
        }
        /*else if(entry->key == key){ // TODO: develop string interning*/
            /*return entry;*/
        /*}*/
        index = (index + 1) % capacity;
    }
}

static void adjustCapacity(Table *table, int capacity){
    Entry *entries = (Entry *)malloc(sizeof(Entry) * capacity);
    for(int i = 0; i < capacity; i++){
        entries[i].key = NULL;
        entries[i].value = (Value){NULL};
    }

    table->count = 0;
    for(int i = 0; i < table->capacity; i++){
        Entry *entry = &table->entries[i];
        if(entry->key == NULL) continue;
        Entry *dest = findEntry(entries, capacity, entry->key);
        dest->key = entry->key;
        dest->value = entry->value;
        table->count++;
    }

    free(table->entries);

    table->entries = entries;
    table->capacity = capacity;
}

void initTable(Table *table){
    table->count = 0;
    table->capacity = 0;
    table->entries = NULL;
}
void freeTable(Table *table){
    free(table->entries);
    initTable(table);
}
bool tableGet(Table *table, ObjString *key, Value *value){
    if(table->count == 0) return false;

    Entry *entry = findEntry(table->entries, table->capacity, key);
    if(entry->key == NULL) return false;
    
    *value = entry->value;
    return true;
}

bool tableSet(Table *table, ObjString *key, Value value){
    if(table->count + 1 > table->capacity){
        int capacity = (table->capacity < 8) ? 8 : table->capacity * 2;
        adjustCapacity(table, capacity);
    }
    Entry *entry = findEntry(table->entries, table->capacity, key);
    bool isNewKey = entry->key == NULL;
    if(isNewKey) table->count++;
    entry->key = key;
    entry->value = value;

    return isNewKey;
}

void printTable(Table *table){
    if(table->count == 0) return;
    for(int i = 0; i < table->capacity; i++){
        Entry *entry = &table->entries[i];
        if(entry->key != NULL){
            printObj((Obj *)entry->key);
            printValue(entry->value);
        }
    } 
}
