#ifndef LUANDARRAY_SRC_CORE_SIMD_SSE_H_
#define LUANDARRAY_SRC_CORE_SIMD_SSE_H_
#ifdef __SSE__
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>

float *LNFloat32Vector_Fill_SSE(float *z, float fill_value, size_t n);
float *LNFloat32Vector_Add_SSE(float *z, const float *a, const float *b, size_t n);
float *LNFloat32Vector_Sub_SSE(float *z, const float *a, const float *b, size_t n);
float *LNFloat32Vector_Mul_SSE(float *z, const float *a, const float *b, size_t n);
float *LNFloat32Vector_Div_SSE(float *z, const float *a, const float *b, size_t n);

#ifdef __SSE2__
double *LNFloat64Vector_Fill_SSE(double *z, double fill_value, size_t n);
double *LNFloat64Vector_Add_SSE(double *z, const double *a, const double *b, size_t n);
double *LNFloat64Vector_Sub_SSE(double *z, const double *a, const double *b, size_t n);
double *LNFloat64Vector_Mul_SSE(double *z, const double *a, const double *b, size_t n);
double *LNFloat64Vector_Div_SSE(double *z, const double *a, const double *b, size_t n);
#endif

#endif

#endif /* LUANDARRAY_SRC_CORE_SIMD_SSE_H_ */