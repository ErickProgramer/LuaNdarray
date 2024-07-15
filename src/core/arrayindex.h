#ifndef LUANDARRAY_SRC_CORE_ARRAYINDEX_H_
#define LUANDARRAY_SRC_CORE_ARRAYINDEX_H_

#include<stdio.h>
#include<stdlib.h>

#include "arrayobj.h"

int LNGetIndexStart();
void LNIndexStartZero(bool_t s);

Ndarray *LNArray_Index(Ndarray *arr, long long idx);
Ndarray *LNArray_Index_(Ndarray *out, Ndarray *arr, long long idx);

// Ndarray *LNArray_MultiIndex_(Ndarray *out, Ndarray *arr, size_t *idxs, size_t nidxs);
Ndarray *LNArray_MultiIndex(Ndarray *arr, long long *idxs, size_t nidxs);

void* LNArray_MultiIndexItem(Ndarray *arr, long long *idxs);

void LNArray_MapDim(Ndarray *arr, long long dim);

#endif