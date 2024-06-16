#ifndef LUANDARRAY_SRC_CORE_SIMD_AVX_H_
#define LUANDARRAY_SRC_CORE_SIMD_AVX_H_
#ifdef __AVX__

#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>

float *LNFloat32Vector_Fill_AVX(float *z, float fill_value, size_t n);
float *LNFloat32Vector_Add_AVX(float *z, const float *a, const float *b, size_t n);
float *LNFloat32Vector_Sub_AVX(float *z, const float *a, const float *b, size_t n);
float *LNFloat32Vector_Mul_AVX(float *z, const float *a, const float *b, size_t n);
float *LNFloat32Vector_Div_AVX(float *z, const float *a, const float *b, size_t n);

double *LNFloat64Vector_Fill_AVX(double *z, double fill_value, size_t n);
double *LNFloat64Vector_Add_AVX(double *z, const double *a, const double *b, size_t n);
double *LNFloat64Vector_Sub_AVX(double *z, const double *a, const double *b, size_t n);
double *LNFloat64Vector_Mul_AVX(double *z, const double *a, const double *b, size_t n);
double *LNFloat64Vector_Div_AVX(double *z, const double *a, const double *b, size_t n);

#endif
#endif