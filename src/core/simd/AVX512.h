#ifndef LUANDARRAY_SRC_CORE_SIMD_AVX512_H_
#define LUANDARRAY_SRC_CORE_SIMD_AVX512_H_

#ifdef __AVX512F__
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>

float *LNFloat32Vector_Fill_AVX512(float *z, float fill_value, size_t n);
float *LNFloat32Vector_Add_AVX512(float *z, const float *a, const float *b, size_t n);
float *LNFloat32Vector_Sub_AVX512(float *z, const float *a, const float *b, size_t n);
float *LNFloat32Vector_Mul_AVX512(float *z, const float *a, const float *b, size_t n);
float *LNFloat32Vector_Div_AVX512(float *z, const float *a, const float *b, size_t n);

double *LNFloat64Vector_Fill_AVX512(double *z, double fill_value, size_t n);
double *LNFloat64Vector_Add_AVX512(double *z, const double *a, const double *b, size_t n);
double *LNFloat64Vector_Sub_AVX512(double *z, const double *a, const double *b, size_t n);
double *LNFloat64Vector_Mul_AVX512(double *z, const double *a, const double *b, size_t n);
double *LNFloat64Vector_Div_AVX512(double *z, const double *a, const double *b, size_t n);

#endif
#endif /* LUANDARRAY_SRC_CORE_SIMD_AVX512_H_ */