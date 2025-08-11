#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "simplex/finite.h"

#include "parser.h"
#include "scanner.h"
#include "table.h"

Parser parser;

static void buildFa(){
    char *faString = NULL;
    int strLength = 0;
    Table *table = &parser.tokenTable;
    if(table->count == 0) return;
    for(int i = 0; i < table->capacity; i++){
        Entry *entry = &table->entries[i];
        if(entry->key != NULL){
            ObjString *str = entry->key;
            if(faString == NULL){
                faString = (char *)malloc(strLength + str->length + 2);
            }else{
                faString = (char *)realloc(faString, strLength + str->length + 2);
            }            
            memcpy(faString + strLength, str->s, str->length);
            strLength = strLength + str->length + 1;
            faString[strLength - 1] = '|';
            faString[strLength] = '\0';

        }
    }

    if(faString == NULL) return;
    faString[strLength - 1] = '\0';
    printf("faString: %s\n", faString);

    parser.fa = buildNfafStr(faString);
}

void initParser(const char *source){
    initTable(&parser.tokenTable);
    initTable(&parser.ruleTable);
    parser.objs = NULL;
    parser.fa = NULL;
    
    
    initScanner(source);
    scan(buildFa);
    
    printf("token table: \n");
    printTable(&parser.tokenTable);
    printf("rule table: \n");
    printTable(&parser.ruleTable);

    freeObjects(parser.objs);
    freeTable(&parser.tokenTable);
    freeTable(&parser.ruleTable);

    if(parser.fa != NULL) freeFa(parser.fa);
}
