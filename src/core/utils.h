#ifndef LUANDARRAY_SRC_CORE_UTILS_H_
#define LUANDARRAY_SRC_CORE_UTILS_H_

#include<stdio.h>
#include<stdlib.h>

#define LN_CONCAT2(a,b) LN_CONCAT2_EXPAND(a,b)
#define LN_CONCAT2_EXPAND(a,b) a##b

#define LN_CONCAT3(a,b,c) LN_CONCAT3_EXPAND(a,b,c)
#define LN_CONCAT3_EXPAND(a,b,c) a##b##c

#define LN_CONCAT4(a,b,c,d) LN_CONCAT4_EXPAND(a,b,c,d)
#define LN_CONCAT4_EXPAND(a,b,c,d) a##b##c##d

#define LN_MAX(x,y) ((x)>(y)) ? (x) : (y)
#define LN_MIN(x,y) ((x)<(y)) ? (x) : (y)

void *LNMem_alloc(size_t bytes);
void *LNMem_calloc(size_t sz, size_t q);
void *LNMem_realloc(void *ptr, size_t newsize);

#endif
