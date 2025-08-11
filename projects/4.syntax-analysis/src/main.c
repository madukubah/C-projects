#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scanner.h"
#include "table.h"
#include "parser.h"

static char *readFIle(const char *path){
    FILE *file = fopen(path, "rb");
    if(file == NULL){
        fprintf(stderr, "Could not open file %s.\n", path);
        exit(74);
    }
    fseek(file, 0L, SEEK_END);
    size_t fileSize = ftell(file);
    rewind(file);

    char *buffer = (char *)malloc(fileSize + 1);
    size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
    if(bytesRead < fileSize){
        fprintf(stderr, "Could not read entire file");
        exit(74);
    }
    buffer[bytesRead] = '\0';
    return buffer;
}

int main(int argc, const char *argv[]){
    
    const char *buff = readFIle("sample.txt");
    printf("buff: %s\n", buff);
   
    initParser(buff);

    /*initScanner(buff);*/
    /*scan();*/
    /*char *s = (char *)malloc(5);*/
    /*char *s1 = (char *)malloc(5);*/
    /*memcpy(s, "key1", 4);*/
    /*memcpy(s1, "val1", 4);*/
    /*s[4] = '\0';*/
    /*s1[4] = '\0';*/

    /*ObjString *key = allocateString(s, 5);*/
    /*printf("%zu \n", key->hash);*/
    /*ObjString *val = allocateString(s1, 5);*/
    /*ObjToken *token1 = allocateToken(val);*/
    /*printf("%zu \n", val->hash);*/
    /*Value value = (Value){(Obj *)token1};*/
    /*printObj((Obj *)key);*/
    /*printValue(value);*/

    /*Table tokenTable;*/
    /*initTable(&tokenTable);*/
    /*tableSet(&tokenTable, key, value);*/

    /*Value find;*/
    /*if(tableGet(&tokenTable, key, &find)){*/
        /*printValue(find);*/
    /*}*/
    return 0;

}
