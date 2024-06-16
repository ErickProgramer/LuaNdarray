#ifdef __AVX__

#include<stdio.h>
#include<stdlib.h>
#include<immintrin.h>

#include "AVX.h"

float *LNFloat32Vector_Fill_AVX(float *z, float fill_value, size_t n){
    __m256 v = _mm256_set1_ps(fill_value);

    size_t i;
    for(i = 0; i < ((n)-(n%8)); i+=8){
        _mm256_storeu_ps(z + i, v);
    }

    for(; i < n; i++){
        z[i] = fill_value;
    }

    return z;
}

float *LNFloat32Vector_Add_AVX(float *z, const float *a, const float *b, size_t n){
    size_t i;

    for(i = 0; i < ((n)-(n%8)); i+=8){
        __m256 _a = _mm256_loadu_ps(a + i);
        __m256 _b = _mm256_loadu_ps(b + i);
        __m256 _z = _mm256_add_ps(_a, _b);
        _mm256_storeu_ps(z + i, _z);
    }

    for(; i < n; i++){
        z[i] = a[i] + b[i];
    }
}

float *LNFloat32Vector_Sub_AVX(float *z, const float *a, const float *b, size_t n){
    size_t i;

    for(i = 0; i < ((n)-(n%8)); i+=8){
        __m256 _a = _mm256_loadu_ps(a + i);
        __m256 _b = _mm256_loadu_ps(b + i);
        __m256 _z = _mm256_sub_ps(_a, _b);
        _mm256_storeu_ps(z + i, _z);
    }

    for(; i < n; i++){
        z[i] = a[i] - b[i];
    }
}

float *LNFloat32Vector_Mul_AVX(float *z, const float *a, const float *b, size_t n){
    size_t i;

    for(i = 0; i < ((n)-(n%8)); i+=8){
        __m256 _a = _mm256_loadu_ps(a + i);
        __m256 _b = _mm256_loadu_ps(b + i);
        __m256 _z = _mm256_mul_ps(_a, _b);
        _mm256_storeu_ps(z + i, _z);
    }

    for(; i < n; i++){
        z[i] = a[i] * b[i];
    }
}

float *LNFloat32Vector_Div_AVX(float *z, const float *a, const float *b, size_t n){
    size_t i;

    for(i = 0; i < ((n)-(n%8)); i+=8){
        __m256 _a = _mm256_loadu_ps(a + i);
        __m256 _b = _mm256_loadu_ps(b + i);
        __m256 _z = _mm256_add_ps(_a, _b);
        _mm256_storeu_ps(z + i, _z);
    }

    for(; i < n; i++){
        z[i] = a[i] + b[i];
    }
}


double *LNFloat64Vector_Fill_AVX(double *z, double fill_value, size_t n){
    __m256d v = _mm256_set1_pd(fill_value);

    size_t i;
    for(i = 0; i < ((n)-(n%8)); i+=8){
        _mm256_storeu_pd(z + i, v);
    }

    for(; i < n; i++){
        z[i] = fill_value;
    }

    return z;
}

double *LNFloat64Vector_Add_AVX(double *z, const double *a, const double *b, size_t n){
    size_t i;

    for(i = 0; i < ((n)-(n%4)); i+=4){
        __m256d _a = _mm256_loadu_pd(a + i);
        __m256d _b = _mm256_loadu_pd(b + i);
        __m256d _z = _mm256_add_pd(_a, _b);
        _mm256_storeu_pd(z + i, _z);
    }

    for(; i < n; i++){
        z[i] = a[i] + b[i];
    }
}

double *LNFloat64Vector_Sub_AVX(double *z, const double *a, const double *b, size_t n){
    size_t i;

    for(i = 0; i < ((n)-(n%4)); i+=4){
        __m256d _a = _mm256_loadu_pd(a + i);
        __m256d _b = _mm256_loadu_pd(b + i);
        __m256d _z = _mm256_sub_pd(_a, _b);
        _mm256_storeu_pd(z + i, _z);
    }

    for(; i < n; i++){
        z[i] = a[i] - b[i];
    }
}

double *LNFloat64Vector_Mul_AVX(double *z, const double *a, const double *b, size_t n){
    size_t i;

    for(i = 0; i < ((n)-(n%4)); i+=4){
        __m256d _a = _mm256_loadu_pd(a + i);
        __m256d _b = _mm256_loadu_pd(b + i);
        __m256d _z = _mm256_mul_pd(_a, _b);
        _mm256_storeu_pd(z + i, _z);
    }

    for(; i < n; i++){
        z[i] = a[i] * b[i];
    }
}

double *LNFloat64Vector_Div_AVX(double *z, const double *a, const double *b, size_t n){
    size_t i;

    for(i = 0; i < ((n)-(n%4)); i+=4){
        __m256d _a = _mm256_loadu_pd(a + i);
        __m256d _b = _mm256_loadu_pd(b + i);
        __m256d _z = _mm256_div_pd(_a, _b);
        _mm256_storeu_pd(z + i, _z);
    }

    for(; i < n; i++){
        z[i] = a[i] / b[i];
    }
}

#endif