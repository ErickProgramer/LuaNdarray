#ifndef LUANDARRAY_SRC_CORE_ARRAYCAST_H_
#define LUANDARRAY_SRC_CORE_ARRAYCAST_H_

#include "arrayobj.h"

typedef enum LNCast_t{
    LNCAST_UNSAFE=0,
    LNCAST_SAFE
} LNCast_t;

void LNArray_CastTo_(Ndarray *out, const Ndarray *arr, const LNTypeDescr *newtype, LNCast_t casttype);
Ndarray *LNArray_CastTo(const Ndarray *arr, const LNTypeDescr *newtype, LNCast_t casttype);
int LNArray_CanCast(const LNTypeDescr *from, const LNTypeDescr *to, LNCast_t casttype);

#endif