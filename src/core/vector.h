// #ifndef LUANDARRAY_MULTIARRAY_VECTOR_H_
// #define LUANDARRAY_MULTIARRAY_VECTOR_H_

// #include <stdio.h>
// #include <stdlib.h>

// #include "simd/SSE.h"
// #include"arraydtype.h"

// // double *LNDoubleVector_Copy(double *z, const double *v, const size_t n);
// float64_t *LNFloat64_Fill(float64_t *z, const float64_t c, const size_t n);
// float64_t *LNFloat64Vector_Add(float64_t *z, const float64_t *a, const float64_t *b, const size_t n);
// float64_t *LNFloat64Vector_Sub(float64_t *z, const float64_t *a, const float64_t *b, const size_t n);
// float64_t *LNFloat64Vector_Mul(float64_t *z, const float64_t *a, const float64_t *b, const size_t n);
// float64_t *LNFloat64Vector_Div(float64_t *z, const float64_t *a, const float64_t *b, const size_t n);

// float32_t *LNFloat32_Fill(float32_t *z, const float32_t c, const size_t n);
// float32_t *LNFloat32Vector_Add(float32_t *z, const float32_t *a, const float32_t *b, const size_t n);
// float32_t *LNFloat32Vector_Sub(float32_t *z, const float32_t *a, const float32_t *b, const size_t n);
// float32_t *LNFloat32Vector_Mul(float32_t *z, const float32_t *a, const float32_t *b, const size_t n);
// float32_t *LNFloat32Vector_Div(float32_t *z, const float32_t *a, const float32_t *b, const size_t n);

// int8_t *LNInt8Vector_Fill(int8_t *v, int8_t fill_value);
// int8_t *LNInt8Vector_Add(int8_t *z, const int8_t *a, const int8_t *b, size_t n);
// int8_t *LNInt8Vector_Sub(int8_t *z, const int8_t *a, const int8_t *b, size_t n);
// int8_t *LNInt8Vector_Mul(int8_t *z, const int8_t *a, const int8_t *b, size_t n);
// int8_t *LNInt8Vector_Div(int8_t *z, const int8_t *a, const int8_t *b, size_t n);

// int16_t *LNInt16Vector_Fill(int16_t *v, int16_t fill_value);
// int16_t *LNInt16Vector_Add(int16_t *z, const int16_t *a, const int16_t *b, size_t n);
// int16_t *LNInt16Vector_Sub(int16_t *z, const int16_t *a, const int16_t *b, size_t n);
// int16_t *LNInt16Vector_Mul(int16_t *z, const int16_t *a, const int16_t *b, size_t n);
// int16_t *LNInt16Vector_Div(int16_t *z, const int16_t *a, const int16_t *b, size_t n);

// int32_t LNInt32Vector_Fill(int32_t *v, int32_t fill_value);
// int32_t *LNInt32Vector_Add(int32_t *z, const int32_t *a, const int32_t *b, size_t n);
// int32_t *LNInt32Vector_Sub(int32_t *z, const int32_t *a, const int32_t *b, size_t n);
// int32_t *LNInt32Vector_Mul(int32_t *z, const int32_t *a, const int32_t *b, size_t n);
// int32_t *LNInt32Vector_Div(int32_t *z, const int32_t *a, const int32_t *b, size_t n);

// int64_t *LNInt64Vector_Fill(int64_t *v, int64_t fill_value);
// int64_t *LNInt64Vector_Add(int64_t *z, const int64_t *a, const int64_t *b, size_t n);
// int64_t *LNInt64Vector_Sub(int64_t *z, const int64_t *a, const int64_t *b, size_t n);
// int64_t *LNInt64Vector_Mul(int64_t *z, const int64_t *a, const int64_t *b, size_t n);
// int64_t *LNInt64Vector_Div(int64_t *z, const int64_t *a, const int64_t *b, size_t n);

// complex128_t *LNComplex128Vector_Fill(complex128_t *v, complex128_t fill_value);
// complex128_t *LNComplex128Vector_Add(complex128_t *z, const complex128_t *a, const complex128_t *b, size_t n);
// complex128_t *LNComplex128Vector_Sub(complex128_t *z, const complex128_t *a, const complex128_t *b, size_t n);
// complex128_t *LNComplex128Vector_Mul(complex128_t *z, const complex128_t *a, const complex128_t *b, size_t n);
// complex128_t *LNComplex128Vector_Div(complex128_t *z, const complex128_t *a, const complex128_t *b, size_t n);
// complex128_t *LNComplex128Vector_IDiv(complex128_t *z, const complex128_t *a, const complex128_t *b, size_t n);
// complex128_t LNComplex128Vector_Dot(const complex128_t *a, const complex128_t *b, size_t n);

// complex64_t *LNComplex64Vector_Fill(complex64_t *v, complex64_t fill_value);
// complex64_t *LNComplex64Vector_Add(complex64_t *z, const complex64_t *a, const complex64_t *b, size_t n);
// complex64_t *LNComplex64Vector_Sub(complex64_t *z, const complex64_t *a, const complex64_t *b, size_t n);
// complex64_t *LNComplex64Vector_Mul(complex64_t *z, const complex64_t *a, const complex64_t *b, size_t n);
// complex64_t *LNComplex64Vector_Div(complex64_t *z, const complex64_t *a, const complex64_t *b, size_t n);
// complex64_t *LNComplex64Vector_IDiv(complex64_t *z, const complex64_t *a, const complex64_t *b, size_t n);
// complex64_t LNComplex64Vector_Dot(const complex64_t *a, const complex64_t *b, size_t n);

// #endif /* LUANDARRAY_MULTIARRAY_VECTOR_H_ */

#ifndef LUANDARRAY_MULTIARRAY_VECTOR_H_
#define LUANDARRAY_MULTIARRAY_VECTOR_H_

#include "utils.h"

#define LNVector_(name) LN_CONCAT4(LN, Real, Vector_, name)

#include "generic/vector.h"
#include "LNGenerateInt8Type.h"

#include "generic/vector.h"
#include "LNGenerateInt16Type.h"

#include "generic/vector.h"
#include "LNGenerateInt32Type.h"

#include "generic/vector.h"
#include "LNGenerateInt64Type.h"

#include "generic/vector.h"
#include "LNGenerateFloat32Type.h"

#include "generic/vector.h"
#include "LNGenerateFloat64Type.h"

#include "generic/vector.h"
#include "LNGenerateComplex64Type.h"

#include "generic/vector.h"
#include "LNGenerateComplex128Type.h"

#undef LNVector_

#endif