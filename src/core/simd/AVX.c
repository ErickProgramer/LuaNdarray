// // #include <stdio.h>
// // #include <stdlib.h>
// // #include <math.h>
// // #include <immintrin.h>

// // #include "SSE.h"

// // double *LNDoubleVector_Copy_SSE(double *z, const double *v, const size_t n){
// //     #if defined(__SSE2__)
// //     __m128d cpy;
// //     size_t i;
// //     for(i = 0; i < ((n)-(n%2)); i+=2){
// //         cpy = _mm_loadu_pd(v + i);
// //         _mm_storeu_pd(z + i, cpy);
// //     }

// //     for(; i < n; i++)
// //         z[i] = v[i];
// //     return z;
// //     #endif
// // }

// // double* LNDoubleVector_Fill_SSE(double *z, const double c, const size_t n){
// //     #if defined(__SSE2__)
// //     __m128d v = _mm_set1_pd(c);
// //     size_t i;
// //     for(i = 0; i < ((n)-(n%2)); i+=2){
// //         _mm_storeu_pd(z + i, v);
// //     }

// //     for(; i < n; i++)
// //         z[i] = c;
    
// //     return z;
// //     #endif
// // }

// // double* LNDoubleVector_Add_SSE(double *z, const double *a, const double *b, const size_t n){
// //     #if defined(__SSE2__)
// //     size_t i;
// //     for(i = 0; i < ((n)-(n%2)); i+=2){
// //         __m128d _a = _mm_loadu_pd(a + i);
// //         __m128d _b = _mm_loadu_pd(b + i);
// //         __m128d _z = _mm_add_pd(_a, _b);
// //         _mm_storeu_pd(z + i, _z);
// //     }

// //     if(n % 2 != 0)
// //         z[n-1] = a[n-1] + b[n-1];

// //     return z;
// //     #endif
// // }

// // double* LNDoubleVector_Sub_SSE(double *z, const double *a, const double *b, const size_t n){
// //     #if defined(__SSE2__)
// //     size_t i;
// //     for(i = 0; i <= ((n)-(n%2)); i+=2){
// //         __m128d _a = _mm_loadu_pd(&a[i]);
// //         __m128d _b = _mm_loadu_pd(&b[i]);
// //         __m128d _z = _mm_sub_pd(_a, _b);
// //         _mm_storeu_pd(&z[i], _z);
// //     }

// //     if(n % 2 != 0)
// //         z[n-1] = a[n-1] - b[n-1];
    
// //     return z;
// //     #endif
// // }

// // double* LNDoubleVector_Mul_SSE(double *z, const double *a, const double *b, const size_t n){
// //     #if defined(__SSE2__)
// //     size_t i;
// //     for(i = 0; i <= ((n)-(n%2)); i+=2){
// //         __m128d _a = _mm_loadu_pd(&a[i]);
// //         __m128d _b = _mm_loadu_pd(&b[i]);
// //         __m128d _z = _mm_mul_pd(_a, _b);
// //         _mm_storeu_pd(&z[i], _z);
// //     }

// //     if(n % 2 != 0)
// //         z[n-1] = a[n-1] * b[n-1];
    
// //     return z;
// //     #endif
// // }

// // double *LNDoubleVector_Div_SSE(double *z, const double *a, const double *b, const size_t n){
// //     #if defined(__SSE2__)
// //     size_t i;
// //     for(i = 0; i <= ((n)-(n%2)); i+=2){
// //         __m128d _a = _mm_loadu_pd(a + i);
// //         __m128d _b = _mm_loadu_pd(b + i);
// //         __m128d _z = _mm_div_pd(_a, _b);
// //         _mm_storeu_pd(&z[i], _z);
// //     }

// //     if(n % 2 != 0)
// //         z[n-1] = a[n-1] / b[n-1];
    
// //     return z;
// //     #endif
// // }

// // double LNDoubleVector_Dot_SSE(const double *a, const double *b, const size_t n){
// //     #if defined(__SSE2__)
// //     size_t i;
// //     __m128d _a, _b;
// //     __m128d sum = _mm_setzero_pd();
// //     for(i = 0; i < ((n)-(n%2)); i+=2){
// //         _a = _mm_loadu_pd(a + i);
// //         _b = _mm_loadu_pd(b + i);
// //         sum = _mm_add_pd(sum, _mm_mul_pd(_a, _b));
// //     }

// //     double out = 0;
// //     double sumvec[2];
// //     _mm_store_pd(sumvec, sum);
// //     out = sumvec[0] + sumvec[1];

// //     if(n % 2 != 0)
// //         out += a[n-1] * b[n-1];

// //     return out;
// //     #endif
// // }

// // double *LNDoubleVector_OrBitwise_SSE(double *z, const double *a, const double *b, const size_t n){
// //     #if defined(__SSE2__)
// //     size_t i;
// //     for(i = 0; i < ((n)-(n%2)); i+=2){
// //         __m128d _a = _mm_loadu_pd(a + i);
// //         __m128d _b = _mm_loadu_pd(b + i);
// //         __m128d _z = _mm_or_pd(_a, _b);
// //         _mm_storeu_pd(z + i, _z);
// //     }

// //     for(; i < n; i++)
// //         z[i] = (long long)a[i] | (long long)b[i];
// //     #endif
// // }

// // double *LNDoubleVector_XorBitwise_SSE(double *z, const double *a, const double *b, const size_t n){
// //     #if defined(__SSE2__)
// //     size_t i;
// //     for(i = 0; i < ((n)-(n%2)); i+=2){
// //         __m128d _a = _mm_loadu_pd(a + i);
// //         __m128d _b = _mm_loadu_pd(b + i);
// //         __m128d _z = _mm_xor_pd(_a, _b);
// //         _mm_storeu_pd(z + i, _z);
// //     }

// //     for(; i < n; i++)
// //         z[i] = (long long)a[i] ^ (long long)b[i];
// //     #endif
// //     return z;
// // }

// // double *LNDoubleVector_AndBitwise_SSE(double *z, const double *a, const double *b, const size_t n){
// //     #if defined(__SSE2__)
// //     size_t i;
// //     for(i = 0; i < ((n)-(n%2)); i+=2){
// //         __m128d _a = _mm_loadu_pd(a + i);
// //         __m128d _b = _mm_loadu_pd(b + i);
// //         __m128d _z = _mm_and_pd(_a, _b);
// //         _mm_storeu_pd(z + i, _z);
// //     }

// //     for(; i < n; i++)
// //         z[i] = (long long)a[i] & (long long)b[i];
// //     #endif
// //     return z;
// // }

// // double *LNDoubleVector_IDiv_SSE(double *z, const double *a, const double *b, const size_t n){
// //     #if defined(__SSE2__)
// //     size_t i;
// //     for(i = 0; i <= ((n)-(n%2)); i+=2){
// //         __m128d _a = _mm_loadu_pd(&a[i]);
// //         __m128d _b = _mm_loadu_pd(&b[i]);
// //         __m128d _z = _mm_floor_pd(_mm_div_pd(_a, _b));
// //         _mm_storeu_pd(&z[i], _z);
// //     }

// //     if(n % 2 != 0)
// //         z[n-1] = floor(a[n-1] / b[n-1]);
// //     return z;
// //     #endif
// // }

#ifdef __AVX__

#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<immintrin.h>

#include"simdutils.h"
#include "SSE.h"

#define NEW_OPERATOR_SIMD_FUNCTION(type, Type, Opname, opname, OP, step, load_sufix, sufix, simd_struct)\
    type *LN##Type##Vector_##Opname##_AVX(type *z, const type *a, const type *b, size_t n){\
        size_t i;\
        simd_struct l1, l2, res;\
        for(i = 0; i < ((n)-(n%step)); i+=step){\
            l1 = _mm256_loadu_##sufix(a + i);\
            l2 = _mm256_loadu_##sufix(b + i);\
            res = _mm256_##opname##_##sufix(l1, l2);\
            _mm256_storeu_##sufix(z + i, res);\
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
NEW_OPERATOR_SIMD_FUNCTION(int8_t, Int8, Add, add, +, 16, si128,epi8, __m128i)
NEW_OPERATOR_SIMD_FUNCTION(int8_t, Int8, Sub, sub, -, 16, si128,epi8, __m256i)

NEW_OPERATOR_SIMD_FUNCTION(int16_t, Int16, Add, add, +, 8, si128,epi16, __m256i)
NEW_OPERATOR_SIMD_FUNCTION(int16_t, Int16, Sub, sub, -, 8, si128,epi16, __m256i)

// GENERATE_TYPE_OPERATORS_INTEGER(int16_t, Int16, 8, si128, epi16, __m128i)
GENERATE_TYPE_OPERATORS_INTEGER(int32_t, Int32, 4, si128, epi32, __m256i)
GENERATE_TYPE_OPERATORS_INTEGER(int64_t, Int64, 2, si128, epi64, __m256i)

GENERATE_TYPE_OPERATORS(float, Float32, 4, ps, ps, __m256)
GENERATE_TYPE_OPERATORS(double, Float64, 2, pd, pd, __m256d)

#define LN_GENERATE_FILL_FUNCTION(type, Type, simd_struct, sufix, step)\
    type *LN##Type##Vector_Fill_AVX(type *z, type v, size_t n){\
        size_t i;\
        simd_struct vec = _mm256_set1_##sufix(v);\
        for(i=0;i<((n)-(n%step));i+=step){\
            _mm256_storeu_##sufix(z+i, vec);\
        }\
        for(;i<n;i++)\
            z[i]=v;\
        return z;\
    }
    
LN_GENERATE_FILL_FUNCTION(int8_t, Int8, __m256i, epi8, 16)
LN_GENERATE_FILL_FUNCTION(int16_t, Int16, __m256i, epi16, 8)
LN_GENERATE_FILL_FUNCTION(int32_t, Int32, __m256i, epi32, 4)
int64_t *LNInt64Vector_Fill_AVX(int64_t *z, int64_t v, size_t n){
    size_t i;
    __m256i vec = _mm256_set1_epi64x(v);
    for(i=0;i<((n)-(n%2));i+=2){
        _mm256_storeu_epi64(z+i, vec);
    }
    for(;i<n;i++)
        z[i]=v;
    return z;
}

LN_GENERATE_FILL_FUNCTION(float, Float32, __m256, ps, 4)
LN_GENERATE_FILL_FUNCTION(double, Float64, __m256d, pd, 2)

#endif