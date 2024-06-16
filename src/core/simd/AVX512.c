#ifdef __AVX512F__

#include<stdio.h>
#include<stdlib.h>
#include<immintrin.h>
#include "AVX512.h"

float *LNFloat32Vector_Fill_AVX512(float *z, float fill_value, size_t n){
    __m512 v = _mm512_set1_ps(fill_value);

    size_t i;
    for(i = 0; i < ((n)-(n%16)); i+=16){
        _mm512_storeu_ps(z + i, v);
    }

    for(; i < n; i++){
        z[i] = fill_value;
    }

    return z;
}

float *LNFloat32Vector_Add_AVX512(float *z, const float *a, const float *b, size_t n){
    size_t i;
    for(i = 0; i < ((n)-(n%16)); i+=16){
        __m512 _a = _mm512_loadu_ps(a + i);
        __m512 _b = _mm512_loadu_ps(b + i);
        __m512 _z = _mm512_add_ps(_a, _b);
        _mm512_storeu_ps(z + i, _z);
    }

    for(; i < n; i++){
        z[i] = a[i] + b[i];
    }

    return z;
}

float *LNFloat32Vector_Sub_AVX512(float *z, const float *a, const float *b, size_t n){
    size_t i;
    for(i = 0; i < ((n)-(n%16)); i+=16){
        __m512 _a = _mm512_loadu_ps(a + i);
        __m512 _b = _mm512_loadu_ps(b + i);
        __m512 _z = _mm512_sub_ps(_a, _b);
        _mm512_storeu_ps(z + i, _z);
    }

    for(; i < n; i++){
        z[i] = a[i] - b[i];
    }

    return z;
}

float *LNFloat32Vector_Mul_AVX512(float *z, const float *a, const float *b, size_t n){
    size_t i;
    for(i = 0; i < ((n)-(n%16)); i+=16){
        __m512 _a = _mm512_loadu_ps(a + i);
        __m512 _b = _mm512_loadu_ps(b + i);
        __m512 _z = _mm512_mul_ps(_a, _b);
        _mm512_storeu_ps(z + i, _z);
    }

    for(; i < n; i++){
        z[i] = a[i] * b[i];
    }

    return z;
}

float *LNFloat32Vector_Div_AVX512(float *z, const float *a, const float *b, size_t n){
    size_t i;
    for(i = 0; i < ((n)-(n%16)); i+=16){
        __m512 _a = _mm512_loadu_ps(a + i);
        __m512 _b = _mm512_loadu_ps(b + i);
        __m512 _z = _mm512_div_ps(_a, _b);
        _mm512_storeu_ps(z + i, _z);
    }

    for(; i < n; i++){
        z[i] = a[i] / b[i];
    }

    return z;
}


double *LNFloat64Vector_Fill_AVX512(double *z, double fill_value, size_t n){
    __m512d v = _mm512_set1_pd(fill_value);

    size_t i;
    for(i = 0; i < ((n)-(n%8)); i+=8){
        _mm512_storeu_pd(z + i, v);
    }

    for(; i < n; i++){
        z[i] = fill_value;
    }

    return z;
}

double *LNFloat64Vector_Add_AVX512(double *z, const double *a, const double *b, size_t n){
    size_t i;
    for(i = 0; i < ((n)-(n%8)); i+=8){
        __m512d _a = _mm512_loadu_pd(a + i);
        __m512d _b = _mm512_loadu_pd(b + i);
        __m512d _z = _mm512_add_pd(_a, _b);
        _mm512_storeu_pd(z + i, _z);
    }

    for(; i < n; i++){
        z[i] = a[i] + b[i];
    }

    return z;
}

double *LNFloat64Vector_Sub_AVX512(double *z, const double *a, const double *b, size_t n){
    size_t i;
    for(i = 0; i < ((n)-(n%8)); i+=8){
        __m512d _a = _mm512_loadu_pd(a + i);
        __m512d _b = _mm512_loadu_pd(b + i);
        __m512d _z = _mm512_sub_pd(_a, _b);
        _mm512_storeu_pd(z + i, _z);
    }

    for(; i < n; i++){
        z[i] = a[i] - b[i];
    }

    return z;
}

double *LNFloat64Vector_Mul_AVX512(double *z, const double *a, const double *b, size_t n){
    size_t i;
    for(i = 0; i < ((n)-(n%8)); i+=8){
        __m512d _a = _mm512_loadu_pd(a + i);
        __m512d _b = _mm512_loadu_pd(b + i);
        __m512d _z = _mm512_mul_pd(_a, _b);
        _mm512_storeu_pd(z + i, _z);
    }

    for(; i < n; i++){
        z[i] = a[i] * b[i];
    }

    return z;
}

double *LNFloat64Vector_Div_AVX512(double *z, const double *a, const double *b, size_t n){
    size_t i;
    for(i = 0; i < ((n)-(n%8)); i+=8){
        __m512d _a = _mm512_loadu_pd(a + i);
        __m512d _b = _mm512_loadu_pd(b + i);
        __m512d _z = _mm512_div_pd(_a, _b);
        _mm512_storeu_pd(z + i, _z);
    }

    for(; i < n; i++){
        z[i] = a[i] / b[i];
    }

    return z;
}

#endif