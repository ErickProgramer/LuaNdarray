#ifndef LUANDARRAY_SRC_CORE_SIMD_SSE_H_
#define LUANDARRAY_SRC_CORE_SIMD_SSE_H_
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<emmintrin.h>

int8_t *LNInt8Vector_Fill_SSE(int8_t *v, int8_t fill_value, size_t n);
int8_t *LNInt8Vector_Add_SSE(int8_t *z, const int8_t *a, const int8_t *b, size_t n);
int8_t *LNInt8Vector_Sub_SSE(int8_t *z, const int8_t *a, const int8_t *b, size_t n);
int8_t *LNInt8Vector_Mul_SSE(int8_t *z, const int8_t *a, const int8_t *b, size_t n);
int8_t *LNInt8Vector_Div_SSE(int8_t *z, const int8_t *a, const int8_t *b, size_t n);

int16_t *LNInt16Vector_Fill_SSE(int16_t *v, int16_t fill_value, size_t n);
int16_t *LNInt16Vector_Add_SSE(int16_t *z, const int16_t *a, const int16_t *b, size_t n);
int16_t *LNInt16Vector_Sub_SSE(int16_t *z, const int16_t *a, const int16_t *b, size_t n);
int16_t *LNInt16Vector_Mul_SSE(int16_t *z, const int16_t *a, const int16_t *b, size_t n);
int16_t *LNInt16Vector_Div_SSE(int16_t *z, const int16_t *a, const int16_t *b, size_t n);

int32_t *LNInt32Vector_Fill_SSE(int32_t *v, int32_t fill_value, size_t n);
int32_t *LNInt32Vector_Add_SSE(int32_t *z, const int32_t *a, const int32_t *b, size_t n);
int32_t *LNInt32Vector_Sub_SSE(int32_t *z, const int32_t *a, const int32_t *b, size_t n);
int32_t *LNInt32Vector_Mul_SSE(int32_t *z, const int32_t *a, const int32_t *b, size_t n);
int32_t *LNInt32Vector_Div_SSE(int32_t *z, const int32_t *a, const int32_t *b, size_t n);

int64_t *LNInt64Vector_Fill_SSE(int64_t *v, int64_t fill_value, size_t n);
int64_t *LNInt64Vector_Add_SSE(int64_t *z, const int64_t *a, const int64_t *b, size_t n);
int64_t *LNInt64Vector_Sub_SSE(int64_t *z, const int64_t *a, const int64_t *b, size_t n);
int64_t *LNInt64Vector_Mul_SSE(int64_t *z, const int64_t *a, const int64_t *b, size_t n);
int64_t *LNInt64Vector_Div_SSE(int64_t *z, const int64_t *a, const int64_t *b, size_t n);

float *LNFloat32Vector_Fill_SSE(float *v, float fill_value, size_t n);
float *LNFloat32Vector_Add_SSE(float *z, const float *a, const float *b, size_t n);
float *LNFloat32Vector_Sub_SSE(float *z, const float *a, const float *b, size_t n);
float *LNFloat32Vector_Mul_SSE(float *z, const float *a, const float *b, size_t n);
float *LNFloat32Vector_Div_SSE(float *z, const float *a, const float *b, size_t n);

double *LNFloat64Vector_Fill_SSE(double *v, double fill_value, size_t n);
double *LNFloat64Vector_Add_SSE(double *z, const double *a, const double *b, size_t n);
double *LNFloat64Vector_Sub_SSE(double *z, const double *a, const double *b, size_t n);
double *LNFloat64Vector_Mul_SSE(double *z, const double *a, const double *b, size_t n);
double *LNFloat64Vector_Div_SSE(double *z, const double *a, const double *b, size_t n);

#endif