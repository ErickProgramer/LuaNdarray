#include<stdio.h>
#include<stdlib.h>
#include "error.h"
#include "utils.h"

void *LNMem_alloc(size_t bytes){
    void *block = malloc(bytes);
    if(!block){
        LNError_setFString("attemp to allocate %zu bytes", bytes);
    }
    return block;
}

void *LNMem_calloc(size_t sz, size_t len){
    void *block = calloc(sz, len);
    if(!block){
        LNError_setFString("attemp to allocate %zu bytes", sz * len);
    }
    return block;
}

void *LNMem_realloc(void *ptr, size_t newsize){
    if(!ptr)
        return LNMem_alloc(newsize);
    
    void *res = realloc(ptr, newsize);
    if(!res)
        LNError_setFString("attemp to allocate %zu bytes", newsize);
    
    return res;
}