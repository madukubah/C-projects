#include <stdio.h>
#include <string.h>

#include "nfa.h"

int main(int argc,const char *argv[]){

    if(argc == 4){
        if(strlen(argv[1]) == 3 && memcmp(argv[1], "nfa", 3) == 0){
            nfa(argv[2], argv[3]);
        }

    }else{
        fprintf(stderr, "Usage: \nsimplex nfa [string] [regex]\nsimplex dfa [string] [regex]\n");
        return 1;
    }
    return 0;
}