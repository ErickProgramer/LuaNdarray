#ifndef LUANDARRAY_SRC_CORE_ARRAYINIS_H_
#define LUANDARRAY_SRC_CORE_ARRAYINIS_H_

#include<stdio.h>
#include<stdlib.h>

#include "arraydtype.h"
#include "arrayobj.h"

Ndarray *LNArray_Empty(const size_t *dims, size_t ndim, const LNTypeDescr *dtype);
Ndarray *LNArray_Zeros(const size_t *dims, size_t ndim, const LNTypeDescr *dtype);
Ndarray *LNArray_Ones(const size_t *dims, size_t ndim, const LNTypeDescr *dtype);
Ndarray *LNArray_Range(double start, double stop, double step, const LNTypeDescr *dtype);

#endif