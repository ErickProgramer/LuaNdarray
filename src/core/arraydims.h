#ifndef LUANDARRAY_SRC_CORE_ARRAYDIMS_H_
#define LUANDARRAY_SRC_CORE_ARRAYDIMS_H_

#include "arrayobj.h"

Ndarray *LNArray_ExpandDims(const Ndarray *arr, const size_t *dims, size_t ndims);

#endif