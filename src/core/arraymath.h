#ifndef LUANDARRAY_SRC_CORE_ARRAYMATH_H_
#define LUANDARRAY_SRC_CORE_ARRAYMATH_H_

#include<stdio.h>
#include<stdlib.h>

// #include"utils.h"
#include"arrayobj.h"


#define LNMath_(name) LN_CONCAT4(LN, Real, Array_, name)
#define LNRealDType LN_CONCAT2(LN, Real)

#include "generic/arraymath.h"
#include "LNGenerateAllTypes.h"

#undef LNMath_
#undef LNRealDType

void LNArray_BroadcastTo(Ndarray *out, Ndarray*arr, size_t *to, size_t nd, char *err_msg);

Ndarray*LNArray_Add(Ndarray*arr1, Ndarray*arr2);

Ndarray*LNArray_MaxAxis(Ndarray*arr, long long axis);
Ndarray*LNArray_Max(Ndarray*arr);

Ndarray*LNArray_MinAxis(Ndarray*arr, long long axis);
Ndarray*LNArray_Min(Ndarray*arr);

Ndarray*LNArray_SumAxis(Ndarray*arr, long long axis);
Ndarray*LNArray_Sum(Ndarray*arr);

#undef LNMath_
#undef LNRealDType

#endif