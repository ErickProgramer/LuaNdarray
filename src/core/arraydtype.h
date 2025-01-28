#ifndef LUANDARRAY_SRC_CORE_ARRAYDTYPE_H_
#define LUANDARRAY_SRC_CORE_ARRAYDTYPE_H_

#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<complex.h>

#include "arraytypes.h"


void LNDType_Init();

LNTypeDescr *LNDType_New(LNDTypes id, CastFunc castfunc, LNAxisOperator max_axis_func, LNAxisOperator min_axis_func, LNAxisOperator sum_axis_func, LNMapOperator max_func, LNMapOperator min_func, LNMapOperator sum_func, char *name, char *fmt, size_t alignment, size_t itemsize);
LNTypeDescr *LNDType_NewFromID(LNDTypes id);
const LNTypeDescr *LNDType_GetFromID(LNDTypes id);
LNTypeDescr *LNDType_Copy(LNTypeDescr *dst, const LNTypeDescr *src);
bool_t LNDType_CastIsSafe(const LNTypeDescr *orig_type, const LNTypeDescr *new_type);
const LNTypeDescr *LNDType_setdefault(const LNTypeDescr *type);
const LNTypeDescr *LNDType_getdefault();

void LNDType_Print(const LNTypeDescr *dtype);
const LNTypeDescr *LNDType_Promote(const LNTypeDescr *t1, const LNTypeDescr *t2);

void LNDType_Free(LNTypeDescr *dtype);

#define LNDType_IsInt(id) ((id) >= LN_INT8 && (id) <= LN_INT64)
#define LNDType_IsUInt(id) ((id) >= LN_UINT8 && (id) <= LN_UINT64)
#define LNDType_IsFloat(id) ((id) >= LN_FLOAT32 && (id) <= LN_FLOAT64)
#define LNDType_IsComplex(id) ((id) >= LN_COMPLEX64 && (id) <= LN_COMPLEX128)
#define LNDType_IsSuperior(dtype1,dtype2) ((dtype1)->id > (dtype1)->id)

#define LNDType_GetInt(id1,id2) LNDType_IsInt(id1) ? (id1) : (id2)

#define LNDTYPE_FLOAT32_FMT "%.14g"
#define LNDTYPE_FLOAT64_FMT "%.14g"

#undef CAN_CAST_TYPE

#endif