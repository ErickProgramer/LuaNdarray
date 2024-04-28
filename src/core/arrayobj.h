#ifndef LUANDARRAY_SRC_CORE_ARRAYOBJ_H_
#define LUANDARRAY_SRC_CORE_ARRAYOBJ_H_

#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "arraytypes.h"

#define LNArray_STEP(arr) ((arr)->step)
#define LNArray_DIMENSIONS(arr) ((arr)->dimensions)
#define LNArray_DIMS(arr) ((arr)->dimensions)
#define LNArray_NDIM(arr) ((arr)->nd)
#define LNArray_SIZE(arr) ((arr)->size)
#define LNArray_LEN(arr) ((arr)->dimensions[0])

#define LNArray_TYPE(arr) ((arr)->dtype)

#define LNArray_ITEMSIZE(arr) (LNArray_TYPE(arr)->itemsize)
#define LNArray_ALIGNMENT(arr) (LNArray_TYPE(arr)->alignment)

void LNArray_Init(Ndarray *arr);

void LNArray_Debug(const Ndarray *arr);

Ndarray *LNArray_New(void *data, size_t *dims, long long *strides, size_t nd, const LNTypeDescr *dtype);
Ndarray *LNArray_NewScalar(const void *scalar, const LNTypeDescr *dtype);
Ndarray *LNArray_New1D(void *data, size_t ax0, const LNTypeDescr *dtype);
Ndarray *LNArray_New2D(void *data, size_t ax0, size_t ax1, const LNTypeDescr *dtype);
Ndarray *LNArray_Copy(const Ndarray *arr);
int LNArray_IsContiguous(const Ndarray *arr);

void LNArray_Free(Ndarray *arr);

#define LNArray_Alloc() ((Ndarray*)LNMem_alloc(sizeof(Ndarray)))
#define LNArray_NewMatrix LNArray_New2D

#define LNArray_IsScalar(arr) ((arr)->nd == 0 && (arr)->size == 1)
#define LNArray_IsEmpty(arr) ((arr)->size == 0)

#endif /* LUANDARRAY_SRC_CORE_ARRAYOBJ_H_ */