#include"error.h"
#include"utils.h"
#include"arraydtype.h"
#include"buffer.h"
#include"arrayaplly.h"

#include "arraymath.h"

#include<math.h>

#define CASTCASE(id, oldtype, real)\
    case id:{\
        LN_ARRAY_APPLY_CONTIG(src,\
            ((real*)dst)[i] = (real)(*(oldtype*)item);\
            i++;\
        );\
        break;\
    }

#define LN_GENERATE_CAST_FUNC(tpy)\
    static void ln_cast_##tpy##_to(Ndarray *dst, const Ndarray *src, LNDTypes newtype){\
        size_t i;\
        const tpy *src_cast = (const tpy*)src;\
        switch (newtype){\
            CASTCASE(LN_INT8, tpy, int8_t)\
            CASTCASE(LN_INT16, tpy, int16_t)\
            CASTCASE(LN_INT32, tpy, int32_t)\
            CASTCASE(LN_INT64, tpy, int64_t)\
\
            CASTCASE(LN_UINT8, tpy, uint8_t)\
            CASTCASE(LN_UINT16, tpy, uint16_t)\
            CASTCASE(LN_UINT32, tpy, uint32_t)\
            CASTCASE(LN_UINT64, tpy, uint64_t)\
\
            case LN_COMPLEX64:\
                LN_ARRAY_APPLY_CONTIG(src,\
                    ((complex64_t*)dst)[i].realp = (float32_t)(*(tpy*)item);\
                    ((complex64_t*)dst)[i].imagp = 0;\
                    i++;\
                );\
                break;\
            case LN_COMPLEX128:\
                LN_ARRAY_APPLY_CONTIG(src,\
                    ((complex128_t*)dst)[i].realp = (float64_t)(*(tpy*)item);\
                    ((complex128_t*)dst)[i].imagp = 0;\
                    i++;\
                );\
                break;\
        }\
    }

#define CASE(id, complex_type, newtype)\
    case id:{\
        LN_ARRAY_APPLY_CONTIG(src,\
            ((newtype*)dst)[i] = (newtype)(*(complex_type*)item).realp;\
            i++;\
        );\
        break;\
    }\

#define LN_GENERATE_COMPLEX_CAST_FUNC(complex_type, parts_type)\
    static void ln_cast_##complex_type##_to(Ndarray *dst, const Ndarray *src, LNDTypes newtype){\
        complex_type *src_cast=(complex_type*)src;\
        size_t i;\
        switch(newtype){\
            CASE(LN_INT8,  complex_type, int8_t);\
            CASE(LN_INT16, complex_type, int16_t);\
            CASE(LN_INT32, complex_type, int32_t);\
            CASE(LN_INT64, complex_type, int64_t);\
\
            CASE(LN_UINT8, complex_type, uint8_t);\
            CASE(LN_UINT16, complex_type, uint16_t);\
            CASE(LN_UINT32, complex_type, uint32_t);\
            CASE(LN_UINT64, complex_type, uint64_t);\
\
            CASE(LN_FLOAT32, complex_type, float32_t);\
            CASE(LN_FLOAT64, complex_type, float64_t);\
\
            CASE(LN_BOOL, complex_type, bool_t);\
            CASE(LN_CHAR, complex_type, char);\
\
            case LN_COMPLEX64:\
                LN_ARRAY_APPLY_CONTIG(src,\
                    ((complex64_t*)dst)[i].realp=(float32_t)((*(complex_type*)item).realp);\
                    ((complex64_t*)dst)[i].imagp=(float32_t)((*(complex_type*)item).imagp);\
                    i++;\
                );\
                break;\
            case LN_COMPLEX128:\
                LN_ARRAY_APPLY_CONTIG(src,\
                    ((complex128_t*)dst)[i].realp=(float64_t)(*(complex_type*)item).realp;\
                    ((complex128_t*)dst)[i].imagp=(float64_t)(*(complex_type*)item).imagp;\
                    i++;\
                );\
                break;\
        }\
    }

#define LN_INTEGER_TO_COMPLEX\
    (dst+i)->real=src_cast[i];\
    (dst+i)->imag=0;\

LN_GENERATE_CAST_FUNC(int8_t)
LN_GENERATE_CAST_FUNC(int16_t)
LN_GENERATE_CAST_FUNC(int32_t)
LN_GENERATE_CAST_FUNC(int64_t)

LN_GENERATE_CAST_FUNC(uint8_t)

LN_GENERATE_CAST_FUNC(uint16_t)

LN_GENERATE_CAST_FUNC(uint32_t)
LN_GENERATE_CAST_FUNC(uint64_t)

LN_GENERATE_CAST_FUNC(float32_t)
LN_GENERATE_CAST_FUNC(float64_t)

LN_GENERATE_CAST_FUNC(char)
LN_GENERATE_CAST_FUNC(bool_t)

LN_GENERATE_COMPLEX_CAST_FUNC(complex64_t, float32_t)
LN_GENERATE_COMPLEX_CAST_FUNC(complex128_t, float64_t)

LNTypeDescr *LNDType_New(LNDTypes id,
                        CastFunc castfunc,
                        LNAxisOperator max_axis_func, LNAxisOperator min_axis_func, LNAxisOperator sum_axis_func,
                        LNMapOperator max_func, LNMapOperator min_func, LNMapOperator sum_func,
                        char *name, char *fmt, size_t alignment, size_t itemsize){
    LNTypeDescr *dtype = LNMem_alloc(sizeof(LNTypeDescr));
    if(!dtype)
        return NULL;

    dtype->max_axis_func = max_axis_func;
    dtype->min_axis_func = min_axis_func;
    dtype->sum_axis_func = sum_axis_func;

    dtype->max_func = max_func;
    dtype->min_func = min_func;
    dtype->sum_func = sum_func;

    dtype->castfunc = castfunc;

    dtype->id = id;

    dtype->name = name;
    dtype->fmt = fmt;
    dtype->alignment = alignment;
    dtype->itemsize = itemsize;

    return dtype;
}

void LNDType_Init(){
    LNInt8 = LNDType_NewFromID(LN_INT8);
    LNInt16 = LNDType_NewFromID(LN_INT16);
    LNInt32 = LNDType_NewFromID(LN_INT32);
    LNInt64 = LNDType_NewFromID(LN_INT64);

    LNUInt8 = LNDType_NewFromID(LN_UINT8);
    LNUInt16 = LNDType_NewFromID(LN_UINT16);
    LNUInt32 = LNDType_NewFromID(LN_UINT32);
    LNUInt64 = LNDType_NewFromID(LN_UINT64);

    LNFloat32 = LNDType_NewFromID(LN_FLOAT32);
    LNFloat64 = LNDType_NewFromID(LN_FLOAT64);

    LNComplex64 = LNDType_NewFromID(LN_COMPLEX64);
    LNComplex128 = LNDType_NewFromID(LN_COMPLEX128);

    LNChar = LNDType_NewFromID(LN_CHAR);
    LNBool = LNDType_NewFromID(LN_BOOL);
}

#define LN_NEW_DTYPE_CREATOR(id, real, Real, repr)\
    LNTypeDescr *LNDType_New##Real(){\
        return LNDType_New(id,\
            ln_cast_##real##_to,\
            LN##Real##Array_MaxAxis, LN##Real##Array_MinAxis, LN##Real##Array_SumAxis,\
            LN##Real##Array_Max, LN##Real##Array_Min, LN##Real##Array_Sum,\
            #repr,\
            NULL, sizeof(real), sizeof(real));\
    }


LN_NEW_DTYPE_CREATOR(LN_INT8, int8_t, Int8, int8)
LN_NEW_DTYPE_CREATOR(LN_INT16, int16_t, Int16, int16)
LN_NEW_DTYPE_CREATOR(LN_INT32, int32_t, Int32, int32)
LN_NEW_DTYPE_CREATOR(LN_INT64, int64_t, Int64, int64)

LN_NEW_DTYPE_CREATOR(LN_UINT8, uint8_t, UInt8, uint8)
LN_NEW_DTYPE_CREATOR(LN_UINT16, uint16_t, UInt16, uint16)
LN_NEW_DTYPE_CREATOR(LN_UINT32, uint32_t, UInt32, uint32)
LN_NEW_DTYPE_CREATOR(LN_UINT64, uint64_t, UInt64, uint64)

LN_NEW_DTYPE_CREATOR(LN_FLOAT32, float32_t, Float32, float64)
LN_NEW_DTYPE_CREATOR(LN_FLOAT64, float64_t, Float64, float64)

LN_NEW_DTYPE_CREATOR(LN_COMPLEX64, complex64_t, Complex64, complex64)
LN_NEW_DTYPE_CREATOR(LN_COMPLEX128, complex128_t, Complex128, complex128)

LNTypeDescr *LNDType_NewBool(){
    return LNDType_New(LN_BOOL,
                       ln_cast_bool_t_to,
                       LNInt8Array_MaxAxis, LNInt8Array_MinAxis,
                       LNInt8Array_SumAxis,
                       LNInt8Array_Max,
                       LNInt8Array_Min,
                       LNInt8Array_Sum,
                       "bool",
                       NULL,
                       sizeof(bool_t), sizeof(bool_t));
}

LNTypeDescr *LNDType_NewChar(){
    return LNDType_New(LN_CHAR,
                       ln_cast_char_to,
                       LNInt8Array_MaxAxis, LNInt8Array_MinAxis,
                       LNInt8Array_SumAxis,
                       LNInt8Array_Max,
                       LNInt8Array_Min,
                       LNInt8Array_Sum,
                       "char",
                       NULL,
                       sizeof(bool_t), sizeof(bool_t));
}

// LN_NEW_DTYPE_CREATOR(LN_BOOL, bool_t, Bool, bool);
// LN_NEW_DTYPE_CREATOR(LN_CHAR, char, Char, char);

#undef LN_NEW_DTYPE_CREATOR

LNTypeDescr *LNDType_NewFromID(LNDTypes id){
    switch (id){
        case LN_INT8:
            return LNDType_NewInt8();
        case LN_INT16:
            return LNDType_NewInt16();
        case LN_INT32:
            return LNDType_NewInt32();
        case LN_INT64:
            return LNDType_NewInt64();
        case LN_UINT8:
            return LNDType_NewUInt8();
        case LN_UINT16:
            return LNDType_NewUInt16();
        case LN_UINT32:
            return LNDType_NewUInt32();
        case LN_UINT64:
            return LNDType_NewUInt64();
        case LN_FLOAT32:
            return LNDType_NewFloat32();
        case LN_FLOAT64:
            return LNDType_NewFloat64();
        case LN_COMPLEX64:
            return LNDType_NewComplex64();
        case LN_COMPLEX128:
            return LNDType_NewComplex128();
        case LN_CHAR:
            return LNDType_NewChar();
        case LN_BOOL:
            return LNDType_NewBool();
        
        default:
            LNError_setFString("type %d do not exist", id);
            return NULL;
    }
}

const LNTypeDescr *LNDType_GetFromID(LNDTypes id){
    switch (id){
        case LN_INT8:
            return LNInt8;
        case LN_INT16:
            return LNInt16;
        case LN_INT32:
            return LNInt32;
        case LN_INT64:
            return LNInt64;
        case LN_UINT8:
            return LNUInt8;
        case LN_UINT16:
            return LNUInt16;
        case LN_UINT32:
            return LNUInt32;
        case LN_UINT64:
            return LNUInt64;
        case LN_FLOAT32:
            return LNFloat32;
        case LN_FLOAT64:
            return LNFloat64;
        case LN_COMPLEX64:
            return LNComplex64;
        case LN_COMPLEX128:
            return LNComplex128;
        case LN_CHAR:
            return LNChar;
        case LN_BOOL:
            return LNBool;
        
        default:
            LNError_setFString("type %d do not exist", id);
            return NULL;
    }
}

LNTypeDescr *LNDType_Copy(LNTypeDescr *dst, const LNTypeDescr *src){
    dst->id = src->id;
    dst->name = src->name;
    dst->fmt = src->fmt;
    dst->alignment = src->alignment;
    dst->itemsize = src->itemsize;
    return dst;
}

const LNTypeDescr *LNDType_Promote(const LNTypeDescr *t1, const LNTypeDescr *t2){
    if(t1->id==t2->id)
        return t1;
    if(LNDType_IsSuperior(t1,t2)){
        if(t1->itemsize==t2->itemsize)
            return LNDType_GetFromID(t1->id+1);
        return t1;
    }

    if(t1->itemsize==t2->itemsize)
        return LNDType_GetFromID(t2->id+1);
    return t2;

    // if(t1->itemsize == t2->itemsize){

    //     if(LNDType_IsComplex(t1->id) || LNDType_IsComplex(t2->id)){
    //         return LNComplex128;
    //     }

    //     if(LNDType_IsFloat(t1->id) || LNDType_IsFloat(t2->id)){
    //         return LNFloat64;
    //     }
    
    //     if(LNDType_IsInt(t1->id) ||  LNDType_IsInt(t2->id)){
    //         return LNInt64;
    //     }

    //     if(LNDType_IsUInt(t1->id) ||  LNDType_IsUInt(t2->id)){
    //         return LNUInt64;
    //     }

    //     // if(LNDType_IsUInt(t1->id) && LNDType_IsInt(t2->id)){
    //     //     if(t2->id==LN_INT64)
    //     //         return LNDType_GetFromID(LN_FLOAT64);
    //     //     return LNDType_GetFromID(t2->id+1);
    //     // }else if(LNDType_IsInt(t1->id) && LNDType_IsUInt(t2->id)){
    //     //     if(t1->id==LN_INT64)
    //     //         return LNDType_GetFromID(LN_FLOAT64);
    //     //     return LNDType_GetFromID(t1->id+1);
    //     // }
    //     return t1;
    // }

    // if(t1->itemsize > t2->itemsize){
    //     if(LNDType_IsUInt(t1->id) && LNDType_IsInt(t2->id)){
    //         if(t1->id == LN_UINT64){
    //             return LNFloat64;
    //         }
    //         return LNDType_GetFromdID(t1->id-LN_INT32);
    //     }
    //     return t1;
    // }

    // if(LNDType_IsUInt(t2->id) && LNDType_IsInt(t1->id)){
    //     if(t2->id == LN_UINT64){
    //         return LNFloat64;
    //     }
    //     return LNDType_GetFromdID(t2->id-LN_INT32);
    // }

    return t2;
    // LNDTypes newtype;
    // if(t1->id==t2->id){
    //     newtype=t1->id;
    //     return LNDType_NewFromID(newtype);
    // }

    // if(t1->itemsize == t2->itemsize){
    //     if(LNDType_IsInt(t1->id) && LNDType_IsUInt(t2->id)){
    //         if(t1->id == LN_INT64)
    //             newtype = LN_FLOAT64;
    //         else
    //             newtype = t1->id+1;
    //     } else if(LNDType_IsInt(t2->id) && LNDType_IsUInt(t1->id)){
    //         if(t2->id == LN_INT64)
    //             newtype = LN_FLOAT64;
    //         else
    //             newtype = t2->id+1;
    //     } else{
    //         newtype = LN_FLOAT64;
    //     }
    // } else{
    //     newtype = t1->itemsize > t2->itemsize ? t1->id : t2->id;
    // }

    // return LNDType_NewFromID(newtype);
}

bool_t LNDType_CastIsSafe(const LNTypeDescr *orig_type, const LNTypeDescr *new_type){
    return orig_type->itemsize >= new_type->itemsize;
}

void LNDType_Print(const LNTypeDescr *dtype){
    printf("dtype(\"%s\", id=%d, fmt=%%%s, alignment=%zu, itemsize=%zu)\n",
        dtype->name, dtype->id, dtype->fmt, dtype->alignment, dtype->itemsize);
}

void LNDType_Free(LNTypeDescr *dtype){
    if(dtype){
        free(dtype->name);
        free(dtype->castfunc);
        free(dtype);
    }
}
