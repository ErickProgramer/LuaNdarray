#ifndef LUANDARRAY_SRC_CORE_CONVERTS_H_
#define LUANDARRAY_SRC_CORE_CONVERTS_H_

#include<stdio.h>
#include<stdlib.h>

#include "arrayobj.h"
#include "buffer.h"

char *LNArray_toString_(LNBuffer *b, Ndarray *arr);
char *LNArray_toString(Ndarray *arr, const char *prefix, const char *sufix);
void LNArray_Print(Ndarray *arr, const char *prefix, const char *sufix);

#define LNArray_SPrint(arr) LNArray_Print(arr, "array(", ")")

#endif