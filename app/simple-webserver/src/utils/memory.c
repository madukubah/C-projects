#include <stdio.h>
#include <stdlib.h>

#include "memory.h"

void *reallocate(void* ptr, size_t oldSize, size_t newSize){
    if(newSize == 0){
        free(ptr);
        return NULL;
    }

    void *result = (void *)realloc(ptr, newSize);
    if(result == NULL){
        fprintf(stderr, "Error: realloc");
        exit(1);
    }
    return result;
}