#ifdef __SSE__

#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include "SSE.h"

#if defined(__SSE4_2__)
#include <nmmintrin.h> // SSE4.2
#elif defined(__SSE4_1__)
#include <smmintrin.h> // SSE4.1
#elif defined(__SSE3__)
#include <pmmintrin.h> // SSE3
#elif defined(__SSE2__)
#include <emmintrin.h> // SSE2
#elif defined(__SSE__)
#include <xmmintrin.h> // SSE
#endif

float *LNFloat32Vector_Fill_SSE(float *z, float fill_value, size_t n){
    __m128 v = _mm_set1_ps(fill_value);

    size_t i;
    for(i = 0; i < ((n)-(n%4)); i+=4){
        _mm_storeu_ps(z + i, v);
    }

    for(; i < n; i++){
        z[i] = fill_value;
    }

    return z;
}

float *LNFloat32Vector_Add_SSE(float *z, const float *a, const float *b, size_t n){
    size_t i;

    for(i = 0; i < ((n)-(n%4)); i+=4){
        __m128 _a = _mm_loadu_ps(a + i);
        __m128 _b = _mm_loadu_ps(b + i);
        __m128 _z = _mm_add_ps(_a, _b);
        _mm_storeu_ps(z + i, _z);
    }

    for(; i < n; i++){
        z[i] = a[i] + b[i];
    }

    return z;
}

float *LNFloat32Vector_Sub_SSE(float *z, const float *a, const float *b, size_t n){
    size_t i;

    for(i = 0; i < ((n)-(n%4)); i+=4){
        __m128 _a = _mm_loadu_ps(a + i);
        __m128 _b = _mm_loadu_ps(b + i);
        __m128 _z = _mm_sub_ps(_a, _b);
        _mm_storeu_ps(z + i, _z);
    }

    for(; i < n; i++){
        z[i] = a[i] - b[i];
    }

    return z;
}

float *LNFloat32Vector_Div_SSE(float *z, const float *a, const float *b, size_t n){
    size_t i;

    for(i = 0; i < ((n)-(n%4)); i+=4){
        __m128 _a = _mm_loadu_ps(a + i);
        __m128 _b = _mm_loadu_ps(b + i);
        __m128 _z = _mm_div_ps(_a, _b);
        _mm_storeu_ps(z + i, _z);
    }

    for(; i < n; i++){
        z[i] = a[i] / b[i];
    }

    return z;
}

float *LNFloat32Vector_Mul_SSE(float *z, const float *a, const float *b, size_t n){
    size_t i;

    for(i = 0; i < ((n)-(n%4)); i+=4){
        __m128 _a = _mm_loadu_ps(a + i);
        __m128 _b = _mm_loadu_ps(b + i);
        __m128 _z = _mm_mul_ps(_a, _b);
        _mm_storeu_ps(z + i, _z);
    }

    for(; i < n; i++){
        z[i] = a[i] * b[i];
    }

    return z;
}

#ifdef __SSE2__

double *LNFloat64Vector_Fill_SSE(double *z, double fill_value, size_t n){
    __m128d v = _mm_set1_pd(fill_value);

    size_t i;
    for(i = 0; i < ((n)-(n%2)); i+=2){
        _mm_storeu_pd(z + i, v);
    }

    for(i=0; i < n; i++){
        z[i] = fill_value;
    }

    return z;
}

double *LNFloat64Vector_Add_SSE(double *z, const double *a, const double *b, size_t n){
    size_t i;

    for(i = 0; i < ((n)-(n%2)); i+=2){
        __m128d _a = _mm_loadu_pd(a + i);
        __m128d _b = _mm_loadu_pd(b + i);
        __m128d _z = _mm_add_pd(_a, _b);
        _mm_storeu_pd(z + i, _z);
    }

    for(; i < n; i++){
        z[i] = a[i] + b[i];
    }

    return z;
}

double *LNFloat64Vector_Sub_SSE(double *z, const double *a, const double *b, size_t n){
    size_t i;

    for(i = 0; i < ((n)-(n%2)); i+=2){
        __m128d _a = _mm_loadu_pd(a + i);
        __m128d _b = _mm_loadu_pd(b + i);
        __m128d _z = _mm_sub_pd(_a, _b);
        _mm_storeu_pd(z + i, _z);
    }

    for(; i < n; i++){
        z[i] = a[i] - b[i];
    }

    return z;
}

double *LNFloat64Vector_Div_SSE(double *z, const double *a, const double *b, size_t n){
    size_t i;

    for(i = 0; i < ((n)-(n%2)); i+=2){
        __m128d _a = _mm_loadu_pd(a + i);
        __m128d _b = _mm_loadu_pd(b + i);
        __m128d _z = _mm_div_pd(_a, _b);
        _mm_storeu_pd(z + i, _z);
    }

    for(; i < n; i++){
        z[i] = a[i] / b[i];
    }

    return z;
}

double *LNFloat64Vector_Mul_SSE(double *z, const double *a, const double *b, size_t n){
    size_t i;

    for(i = 0; i < ((n)-(n%4)); i+=4){
        __m128d _a = _mm_loadu_pd(a + i);
        __m128d _b = _mm_loadu_pd(b + i);
        __m128d _z = _mm_mul_pd(_a, _b);
        _mm_storeu_pd(z + i, _z);
    }

    for(; i < n; i++){
        z[i] = a[i] * b[i];
    }

    return z;
}

#endif

#endif
