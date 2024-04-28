#if 0
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <immintrin.h>

#include "AVX2.h"

double *LNDoubleVector_Copy_AVX2(double *z, const double *v, const size_t n){
    #if defined(__AVX2__)
    size_t i;
    for(i = 0; i < ((n)-(n%8)); i+=8){
        __m512d _a = _mm512_loadu_pd(v + i);
        _mm512_storeu_pd(z + i, _a);
    }

    for(; i < n; i++)
        z[i] = v[i];
    #endif
    return z;
}

double *LNDoubleVector_Fill_AVX2(double *z, const double c, const size_t n){
    #if defined(__AVX2__)
    __m512d v = _mm512_set1_pd(c);
    size_t i;
    for(i = 0; i < ((n)-(n%8)); i+=8)
        _mm512_storeu_pd(z + i, v);
    
    for(; i < n; i++)
        z[i] = c;
    #endif
    return z;
}

double *LNDoubleVector_Add_AVX2(double *z, const double *a, const double *b, const size_t n){
    #if defined(__AVX2__)
    size_t i;
    for(i = 0; i <= ((n)-(n%8)); i+=8){
        __m512d _a = _mm512_loadu_pd(a + i);
        __m512d _b = _mm512_loadu_pd(b + i);
        __m512d _z = _mm512_add_pd(_a, _b);
        _mm512_storeu_pd(z + i, _z);
    }

    for(; i < n; i++)
        z[i] = a[i] + b[i];
    return z;
    #endif
}

double *LNDoubleVector_Sub_AVX2(double *z, const double *a, const double *b, const size_t n){
    #if defined(__AVX2__)
    size_t i;
    for(i = 0; i <= ((n)-(n%8)); i++){
        __m512d _a = _mm512_loadu_pd(a + i);
        __m512d _b = _mm512_loadu_pd(b + i);
        __m512d _z = _mm512_sub_pd(_a, _b);
        _mm512_storeu_pd(z + i, _z);
    }

    for(; i < n; i++)
        z[i] = a[i] - b[i];
    return z;
    #endif
}

double *LNDoubleVector_Mul_AVX2(double *z, const double *a, const double *b, const size_t n){
    #if defined(__AVX2__)
    size_t i;
    for(i = 0; i <= ((n)-(n%8)); i++){
        __m512d _a = _mm512_loadu_pd(a + i);
        __m512d _b = _mm512_loadu_pd(b + i);
        __m512d _z = _mm512_mul_pd(_a, _b);
        _mm512_storeu_pd(z + i, _z);
    }

    for(; i < n; i++)
        z[i] = a[i] * b[i];
    return z;
    #endif
}

double *LNDoubleVector_Div_AVX2(double *z, const double *a, const double *b, const size_t n){
    #if defined(__AVX2__)
    size_t i;
    for(i = 0; i <= ((n)-(n%8)); i++){
        __m512d _a = _mm512_loadu_pd(a + i);
        __m512d _b = _mm512_loadu_pd(b + i);
        __m512d _z = _mm512_div_pd(_a, _b);
        _mm512_storeu_pd(z + i, _z);
    }

    for(; i < n; i++)
        z[i] = a[i] + b[i];
    return z;
    #endif
}

double *LNDoubleVector_IDiv_AVX2(double *z, const double *a, const double *b, const size_t n){
    #if defined(__AVX2__)
    size_t i;
    for(i = 0; i <= ((n)-(n%8)); i++){
        __m512d _a = _mm512_loadu_pd(a + i);
        __m512d _b = _mm512_loadu_pd(b + i);
        __m512d _z = _mm512_add_pd(_a, _b);
        _z = _mm512_floor_pd(_z);
        _mm512_storeu_pd(z + i, _z);
    }

    for(; i < n; i++)
        z[i] = floor(a[i] / b[i]);
    return z;
    #endif
}

double LNDoubleVector_Dot_AVX2(const double *a, const double *b, const size_t n){
    #if defined(__AVX2__)
    size_t i;
    __m512d _a, _b;
    __m512d sum = _mm512_setzero_pd();
    for(i = 0; i < ((n)-(n%8)); i+=8){
        _a = _mm512_loadu_pd(a + i);
        _b = _mm512_loadu_pd(b + i);
        sum = _mm512_add_pd(sum, _mm512_mul_pd(_a, _b));
    }

    double out = 0;
    double sumvec[2];
    _mm512_store_pd(sumvec, sum);
    out = sumvec[0] + sumvec[1];

    for(; i < n; i++)
        out += a[i] * b[i];

    return out;
    #endif
}

double *LNDoubleVector_OrBitwise_AVX2(double *z, const double *a, const double *b, const size_t n){
    #if defined(__AVX2__)
    size_t i;
    for(i = 0; i < ((n)-(n%8)); i+=8){
        __m512d _a = _mm512_loadu_pd(a + i);
        __m512d _b = _mm512_loadu_pd(b + i);
        __m512d _z = _mm512_or_pd(_a, _b);
        _mm512_storeu_pd(z + i, _z);
    }

    for(; i < n; i++)
        z[i] = (long long)a[i] | (long long)b[i];
    #endif
    return z;
}

double *LNDoubleVector_XorBitwise_AVX2(double *z, const double *a, const double *b, const size_t n){
    #if defined(__AVX2__)
    size_t i;
    for(i = 0; i < ((n)-(n%8)); i+=8){
        __m512d _a = _mm512_loadu_pd(a + i);
        __m512d _b = _mm512_loadu_pd(b + i);
        __m512d _z = _mm512_xor_pd(_a, _b);
        _mm512_storeu_pd(z + i, _z);
    }

    for(; i < n; i++)
        z[i] = (long long)a[i] ^ (long long)b[i];
    #endif
    return z;
}

double *LNDoubleVector_AndBitwise_AVX2(double *z, const double *a, const double *b, const size_t n){
    #if defined(__AVX2__)
    size_t i;
    for(i = 0; i < ((n)-(n%8)); i+=8){
        __m512d _a = _mm512_loadu_pd(a + i);
        __m512d _b = _mm512_loadu_pd(b + i);
        __m512d _z = _mm512_and_pd(_a, _b);
        _mm512_storeu_pd(z + i, _z);
    }

    for(; i < n; i++)
        z[i] = (long long)a[i] & (long long)b[i];
    #endif
    return z;
}
#endif