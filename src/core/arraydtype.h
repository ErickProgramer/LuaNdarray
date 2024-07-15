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

void LNDType_Print(const LNTypeDescr *dtype);
const LNTypeDescr *LNDType_Promote(const LNTypeDescr *t1, const LNTypeDescr *t2);

void LNDType_Free(LNTypeDescr *dtype);

#define LNDType_IsInt(id) ((id) >= LN_INT8 && (id) <= LN_INT64)
#define LNDType_IsUInt(id) ((id) >= LN_UINT8 && (id) <= LN_UINT64)
#define LNDType_IsFloat(id) ((id) >= LN_FLOAT32 && (id) <= LN_FLOAT64)
#define LNDType_IsComplex(id) ((id) >= LN_COMPLEX64 && (id) <= LN_COMPLEX128)
#define LNDType_IsSuperior(dtype1,dtype2) ((dtype1)->id > (dtype1)->id)

#define LNDType_GetInt(id1,id2) LNDType_IsInt(id1) ? (id1) : (id2)


// #define LNDType_NewInt8() LNDType_New(LN_INT8, "int8", "d", sizeof(int8_t), sizeof(int8_t))
// #define LNDType_NewInt16() LNDType_New(LN_INT16, "int16", "d", sizeof(int16_t), sizeof(int16_t))
// #define LNDType_NewInt32() LNDType_New(LN_INT32, "int32", "d", sizeof(int32_t), sizeof(int32_t))
// #define LNDType_NewInt64() LNDType_New(LN_INT64, "int64", "lld", sizeof(int64_t), sizeof(int64_t))

// #define LNDType_NewUInt8() LNDType_New(LN_UINT8, "uint8", "u", sizeof(uint8_t), sizeof(uint8_t))
// #define LNDType_NewUInt16() LNDType_New(LN_UINT16, "uint16", "u", sizeof(uint16_t), sizeof(uint16_t))
// #define LNDType_NewUInt32() LNDType_New(LN_UINT32, "uint32", "u", sizeof(uint32_t), sizeof(uint32_t))
// #define LNDType_NewUInt64() LNDType_New(LN_UINT64, "uint64", "llu", sizeof(uint64_t), sizeof(uint64_t))

// #define LNDType_NewFloat32() LNDType_New(LN_FLOAT32, "float32", "f", sizeof(float), sizeof(float))
// #define LNDType_NewFloat64() LNDType_New(LN_FLOAT64, "float64", "g", sizeof(double), sizeof(double))

#define LNDTYPE_FLOAT32_FMT "%.14g"
#define LNDTYPE_FLOAT64_FMT "%.14g"



// #define CAN_CAST_TYPE(type) {sizeof(int8_t) >= sizeof(type),\
//                              sizeof(int16_t) >= sizeof(type),\
//                              sizeof(int32_t) >= sizeof(type),\
//                              sizeof(int64_t) >= sizeof(type),\
//                              sizeof(uint8_t) >= sizeof(type),\
//                              sizeof(uint16_t) >= sizeof(type),\
//                              sizeof(uint32_t) >= sizeof(type),\
//                              sizeof(uint64_t) >= sizeof(type),\
//                              sizeof(float) >= sizeof(type),\
//                              sizeof(double) >= sizeof(type)\
//                             }

// LNDTypes CAN_CAST_TABLE[LN_NUMTYPES][LN_NUMTYPES]={
//     CAN_CAST_TYPE(int8_t),
//     CAN_CAST_TYPE(int16_t),
//     CAN_CAST_TYPE(int32_t),
//     CAN_CAST_TYPE(int64_t),

//     CAN_CAST_TYPE(uint8_t),
//     CAN_CAST_TYPE(uint16_t),
//     CAN_CAST_TYPE(uint32_t),
//     CAN_CAST_TYPE(uint64_t),

//     CAN_CAST_TYPE(float),
//     CAN_CAST_TYPE(double),
// };

#undef CAN_CAST_TYPE

#endif