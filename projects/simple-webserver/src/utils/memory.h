#ifndef simple_memory_h
#define simple_memory_h

#include <stdio.h>

#define ALLOCATE(type, count)\
     (type *)reallocate(NULL, 0, sizeof(type) * count)
#define REALLOCATE(type, ptr, oldCount, newCount)\
     (type *)reallocate(ptr, sizeof(type) * oldCount, sizeof(type) * newCount)
#define FREE(type, ptr)\
     reallocate(ptr, sizeof(type), 0)
    

void *reallocate(void* ptr, size_t oldSize, size_t newSize);

#endif