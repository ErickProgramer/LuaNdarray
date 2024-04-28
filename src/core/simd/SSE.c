
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<immintrin.h>

#include"simdutils.h"
#include "SSE.h"

#define NEW_OPERATOR_SIMD_FUNCTION(type, Type, Opname, opname, OP, step, load_sufix, sufix, simd_struct)\
    type *LN##Type##Vector_##Opname##_SSE(type *z, const type *a, const type *b, size_t n){\
        size_t i;\
        simd_struct l1, l2, res;\
        for(i = 0; i < ((n)-(n%step)); i+=step){\
            l1 = _mm_loadu_##sufix(a + i);\
            l2 = _mm_loadu_##sufix(b + i);\
            res = _mm_##opname##_##sufix(l1, l2);\
            _mm_storeu_##sufix(z + i, res);\
        }\
\
        for(; i < n; i++)\
            z[i] = a[i] OP b[i];\
\
        return z;\
    }

#define GENERATE_TYPE_OPERATORS(type, Type, step, load_sufix, sufix, simd_struct)\
    NEW_OPERATOR_SIMD_FUNCTION(type, Type, Add, add, +, step, load_sufix,sufix, simd_struct)\
    NEW_OPERATOR_SIMD_FUNCTION(type, Type, Sub, sub, -, step, load_sufix,sufix, simd_struct)\
    NEW_OPERATOR_SIMD_FUNCTION(type, Type, Mul, mul, *, step, load_sufix,sufix, simd_struct)\
    NEW_OPERATOR_SIMD_FUNCTION(type, Type, Div, div, /, step, load_sufix,sufix, simd_struct)\

#define GENERATE_TYPE_OPERATORS_INTEGER(type, Type, step, load_sufix, sufix, simd_struct)\
    NEW_OPERATOR_SIMD_FUNCTION(type, Type, Add, add, +, step, load_sufix,sufix, simd_struct)\
    NEW_OPERATOR_SIMD_FUNCTION(type, Type, Sub, sub, -, step, load_sufix,sufix, simd_struct)\
    NEW_OPERATOR_SIMD_FUNCTION(type, Type, Mul, mul, *, step, load_sufix,sufix, simd_struct)\
    NEW_OPERATOR_SIMD_FUNCTION(type, Type, Div, div, *, step, load_sufix,sufix, simd_struct)\

// NEW_OPERATOR_SIMD_FUNCTION(type, Type, Sub, sub, -, step, load_sufix,sufix, simd_struct)\
// GENERATE_TYPE_OPERATORS_INTEGER(int8_t, Int8, 16, si128, epi8, __m128i)
NEW_OPERATOR_SIMD_FUNCTION(int8_t, Int8, Add, add, +, 16, si128,epi8, __m128i)
NEW_OPERATOR_SIMD_FUNCTION(int8_t, Int8, Sub, sub, -, 16, si128,epi8, __m128i)

NEW_OPERATOR_SIMD_FUNCTION(int16_t, Int16, Add, add, +, 8, si128,epi16, __m128i)
NEW_OPERATOR_SIMD_FUNCTION(int16_t, Int16, Sub, sub, -, 8, si128,epi16, __m128i)

// GENERATE_TYPE_OPERATORS_INTEGER(int16_t, Int16, 8, si128, epi16, __m128i)
GENERATE_TYPE_OPERATORS_INTEGER(int32_t, Int32, 4, si128, epi32, __m128i)
GENERATE_TYPE_OPERATORS_INTEGER(int64_t, Int64, 2, si128, epi64, __m128i)

GENERATE_TYPE_OPERATORS(float, Float32, 4, ps, ps, __m128)
GENERATE_TYPE_OPERATORS(double, Float64, 2, pd, pd, __m128d)

#define LN_GENERATE_FILL_FUNCTION(type, Type, simd_struct, sufix, step)\
    type *LN##Type##Vector_Fill_SSE(type *z, type v, size_t n){\
        size_t i;\
        simd_struct vec = _mm_set1_##sufix(v);\
        for(i=0;i<((n)-(n%step));i+=step){\
            _mm_storeu_##sufix(z+i, vec);\
        }\
        for(;i<n;i++)\
            z[i]=v;\
        return z;\
    }
    
LN_GENERATE_FILL_FUNCTION(int8_t, Int8, __m128i, epi8, 16)
LN_GENERATE_FILL_FUNCTION(int16_t, Int16, __m128i, epi16, 8)
LN_GENERATE_FILL_FUNCTION(int32_t, Int32, __m128i, epi32, 4)
int64_t *LNInt64Vector_Fill_SSE(int64_t *z, int64_t v, size_t n){
    size_t i;
    __m128i vec = _mm_set1_epi64((__m64)v);
    for(i=0;i<((n)-(n%2));i+=2){
        _mm_storeu_epi64(z+i, vec);
    }
    for(;i<n;i++)
        z[i]=v;
    return z;
}

LN_GENERATE_FILL_FUNCTION(float, Float32, __m128, ps, 4)
LN_GENERATE_FILL_FUNCTION(double, Float64, __m128d, pd, 2)


