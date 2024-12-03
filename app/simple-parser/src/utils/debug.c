#include <stdio.h>
#include "debug.h"

void printOpList(OperationList opList){
    printf("\n\n");

    for(int i = 0; i < opList.count; i++){
        Operation op = opList.operations[i];
        switch(op.opcode){
            case OP_LITERAL:
                printf("Literal: %c\n", op.value);
                break;
            case OP_CONCAT:
                printf("CONCAT\n");
                break;
            case OP_KLEEN:
                printf("KLEEN\n");
                break;
            case OP_PIPE:
                printf("PIPE\n");
                break;
        }
    }

    printf("\n\n");
}