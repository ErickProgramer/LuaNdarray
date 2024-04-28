// #include <stdio.h>
// #include <stdlib.h>
// #include <math.h>

// #include "vector.h"

// #include "simd/AVX2.h"
// #include "simd/AVX.h"
// #include "simd/SSE.h"
// #include "arraydtype.h"

// int8_t *LNInt8Vector_Fill(int8_t *v, int8_t fill_value){
//     #if defined(__AVX2__)
//     LNInt8Vector_Fill_AVX2(z,a,b,n);
//     #elif defined(__AVX__)
//     LNInt8Vector_Fill_AVX(z,a,b,n);
//     #elif defined(__SSE__)
//     LNInt8Vector_Fill_SSE(z,a,b,n);
//     #else
//     size_t i;
//     for(i=0;i<n;i++)
//         v[i]=fill_value;
//     #endif
//     return z;
// }

// int8_t *LNInt8Vector_Add(int8_t *z, const int8_t *a, const int8_t *b, size_t n){
//     #if defined(__AVX2__)
//     LNInt8Vector_Add_AVX2(z,a,b,n);
//     #elif defined(__AVX__)
//     LNInt8Vector_Add_AVX(z,a,b,n);
//     #elif defined(__SSE__)
//     LNInt8Vector_Add_SSE(z,a,b,n);
//     #else
//     size_t i;
//     for(i=0;i<n;i++)
//         z[i]=a[i]+b[i];
//     #endif
//     return z;
// }

// int8_t *LNInt8Vector_Sub(int8_t *z, const int8_t *a, const int8_t *b, size_t n){
//     #if defined(__AVX2__)
//     LNInt8Vector_Sub_AVX2(z,a,b,n);
//     #elif defined(__AVX__)
//     LNInt8Vector_Sub_AVX(z,a,b,n);
//     #elif defined(__SSE__)
//     LNInt8Vector_Sub_SSE(z,a,b,n);
//     #else
//     size_t i;
//     for(i=0;i<n;i++)
//         z[i]=a[i]-b[i];
//     #endif
//     return z;
// }

// int8_t *LNInt8Vector_Div(int8_t *z, const int8_t *a, const int8_t *b, size_t n){
//     #if defined(__AVX2__)
//     LNInt8Vector_Div_AVX2(z,a,b,n);
//     #elif defined(__AVX__)
//     LNInt8Vector_Div_AVX(z,a,b,n);
//     #elif defined(__SSE__)
//     // LNInt8Vector_Div_SSE(z,a,b,n);
//     #else
//     size_t i;
//     for(i=0;i<n;i++)
//         z[i]=a[i]/b[i];
//     #endif
//     return z;
// }

// int8_t *LNInt8Vector_Mul(int8_t *z, const int8_t *a, const int8_t *b, size_t n){
//     #if defined(__AVX2__)
//     LNInt8Vector_Mul_AVX2(z,a,b,n);
//     #elif defined(__AVX__)
//     LNInt8Vector_Mul_AVX(z,a,b,n);
//     #elif defined(__SSE__)
//     // LNInt8Vector_Mul_SSE(z,a,b,n);
//     #else
//     size_t i;
//     for(i=0;i<n;i++)
//         z[i]=a[i]*b[i];
//     #endif
//     return z;
// }

// int8_t *LNInt16Vector_Fill(int16_t *v, int16_t fill_value){
//     #if defined(__AVX2__)
//     LNInt8Vector_Fill_AVX2(z,a,b,n);
//     #elif defined(__AVX__)
//     LNInt8Vector_Fill_AVX(z,a,b,n);
//     #elif defined(__SSE__)
//     LNInt8Vector_Fill_SSE(z,a,b,n);
//     #else
//     size_t i;
//     for(i=0;i<n;i++)
//         v[i]=fill_value;
//     #endif
//     return z;
// }

// int16_t *LNInt16Vector_Add(int16_t *z, const int16_t *a, const int16_t *b, size_t n){
//     #if defined(__AVX2__)
//     LNInt16Vector_Add_AVX2(z,a,b,n);
//     #elif defined(__AVX__)
//     LNInt16Vector_Add_AVX(z,a,b,n);
//     #elif defined(__SSE__)
//     LNInt16Vector_Add_SSE(z,a,b,n);
//     #else
//     size_t i;
//     for(i=0;i<n;i++)
//         z[i]=a[i]+b[i];
//     #endif
//     return z;
// }

// int16_t *LNInt16Vector_Sub(int16_t *z, const int16_t *a, const int16_t *b, size_t n){
//     #if defined(__AVX2__)
//     LNInt16Vector_Sub_AVX2(z,a,b,n);
//     #elif defined(__AVX__)
//     LNInt16Vector_Sub_AVX(z,a,b,n);
//     #elif defined(__SSE__)
//     LNInt16Vector_Sub_SSE(z,a,b,n);
//     #else
//     size_t i;
//     for(i=0;i<n;i++)
//         z[i]=a[i]-b[i];
//     #endif
//     return z;
// }

// int16_t *LNInt16Vector_Div(int16_t *z, const int16_t *a, const int16_t *b, size_t n){
//     #if defined(__AVX2__)
//     LNInt16Vector_Div_AVX2(z,a,b,n);
//     #elif defined(__AVX__)
//     LNInt16Vector_Div_AVX(z,a,b,n);
//     #elif defined(__SSE__)
//     // LNInt16Vector_Div_SSE(z,a,b,n);
//     #else
//     size_t i;
//     for(i=0;i<n;i++)
//         z[i]=a[i]/b[i];
//     #endif
//     return z;
// }

// int16_t *LNInt16Vector_Mul(int16_t *z, const int16_t *a, const int16_t *b, size_t n){
//     #if defined(__AVX2__)
//     LNInt16Vector_Mul_AVX2(z,a,b,n);
//     #elif defined(__AVX__)
//     LNInt16Vector_Mul_AVX(z,a,b,n);
//     #elif defined(__SSE__)
//     // LNInt16Vector_Mul_SSE(z,a,b,n);
//     #else
//     size_t i;
//     for(i=0;i<n;i++)
//         z[i]=a[i]*b[i];
//     #endif
//     return z;
// }

// int8_t *LNInt32Vector_Fill(int32_t *v, int32_t fill_value){
//     #if defined(__AVX2__)
//     LNInt32Vector_Fill_AVX2(z,a,b,n);
//     #elif defined(__AVX__)
//     LNInt32Vector_Fill_AVX(z,a,b,n);
//     #elif defined(__SSE__)
//     LNInt32Vector_Fill_SSE(z,a,b,n);
//     #else
//     size_t i;
//     for(i=0;i<n;i++)
//         v[i]=fill_value;
//     #endif
//     return z;
// }

// int32_t *LNInt32Vector_Add(int32_t *z, const int32_t *a, const int32_t *b, size_t n){
//     #if defined(__AVX2__)
//     LNInt32Vector_Add_AVX2(z,a,b,n);
//     #elif defined(__AVX__)
//     LNInt32Vector_Add_AVX(z,a,b,n);
//     #elif defined(__SSE__)
//     LNInt32Vector_Add_SSE(z,a,b,n);
//     #else
//     size_t i;
//     for(i=0;i<n;i++)
//         z[i]=a[i]+b[i];
//     #endif
//     return z;
// }

// int32_t *LNInt32Vector_Sub(int32_t *z, const int32_t *a, const int32_t *b, size_t n){
//     #if defined(__AVX2__)
//     LNInt32Vector_Sub_AVX2(z,a,b,n);
//     #elif defined(__AVX__)
//     LNInt32Vector_Sub_AVX(z,a,b,n);
//     #elif defined(__SSE__)
//     LNInt32Vector_Sub_SSE(z,a,b,n);
//     #else
//     size_t i;
//     for(i=0;i<n;i++)
//         z[i]=a[i]-b[i];
//     #endif
//     return z;
// }

// int32_t *LNInt32Vector_Div(int32_t *z, const int32_t *a, const int32_t *b, size_t n){
//     #if defined(__AVX2__)
//     LNInt32Vector_Div_AVX2(z,a,b,n);
//     #elif defined(__AVX__)
//     LNInt32Vector_Div_AVX(z,a,b,n);
//     #elif defined(__SSE__)
//     // LNInt32Vector_Div_SSE(z,a,b,n);
//     #else
//     size_t i;
//     for(i=0;i<n;i++)
//         z[i]=a[i]/b[i];
//     #endif
//     return z;
// }

// int32_t *LNInt32Vector_Mul(int32_t *z, const int32_t *a, const int32_t *b, size_t n){
//     #if defined(__AVX2__)
//     LNInt32Vector_Mul_AVX2(z,a,b,n);
//     #elif defined(__AVX__)
//     LNInt32Vector_Mul_AVX(z,a,b,n);
//     #elif defined(__SSE__)
//     LNInt32Vector_Mul_SSE(z,a,b,n);
//     #else
//     size_t i;
//     for(i=0;i<n;i++)
//         z[i]=a[i]*b[i];
//     #endif
//     return z;
// }

// int64_t *LNInt64Vector_Fill(int64_t *v, int64_t fill_value){
//     #if defined(__AVX2__)
//     LNInt64Vector_Fill_AVX2(z,a,b,n);
//     #elif defined(__AVX__)
//     LNInt64Vector_Fill_AVX(z,a,b,n);
//     #elif defined(__SSE__)
//     LNInt64Vector_Fill_SSE(z,a,b,n);
//     #else
//     size_t i;
//     for(i=0;i<n;i++)
//         v[i]=fill_value;
//     #endif
//     return z;
// }

// int64_t *LNInt64Vector_Add(int64_t *z, const int64_t *a, const int64_t *b, size_t n){
//     #if defined(__AVX2__)
//     LNInt64Vector_Add_AVX2(z,a,b,n);
//     #elif defined(__AVX__)
//     LNInt64Vector_Add_AVX(z,a,b,n);
//     #elif defined(__SSE__)
//     LNInt64Vector_Add_SSE(z,a,b,n);
//     #else
//     size_t i;
//     for(i=0;i<n;i++)
//         z[i]=a[i]+b[i];
//     #endif
//     return z;
// }

// int64_t *LNInt64Vector_Sub(int64_t *z, const int64_t *a, const int64_t *b, size_t n){
//     #if defined(__AVX2__)
//     LNInt64Vector_Sub_AVX2(z,a,b,n);
//     #elif defined(__AVX__)
//     LNInt64Vector_Sub_AVX(z,a,b,n);
//     #elif defined(__SSE__)
//     LNInt64Vector_Sub_SSE(z,a,b,n);
//     #else
//     size_t i;
//     for(i=0;i<n;i++)
//         z[i]=a[i]-b[i];
//     #endif
//     return z;
// }

// int64_t *LNInt64Vector_Div(int64_t *z, const int64_t *a, const int64_t *b, size_t n){
//     #if defined(__AVX2__)
//     LNInt64Vector_Div_AVX2(z,a,b,n);
//     #elif defined(__AVX__)
//     LNInt64Vector_Div_AVX(z,a,b,n);
//     #elif defined(__SSE__)
//     LNInt64Vector_Div_SSE(z,a,b,n);
//     #else
//     size_t i;
//     for(i=0;i<n;i++)
//         z[i]=a[i]/b[i];
//     #endif
//     return z;
// }

// int64_t *LNInt64Vector_Mul(int64_t *z, const int64_t *a, const int64_t *b, size_t n){
//     #if defined(__AVX2__)
//     LNInt64Vector_Mul_AVX2(z,a,b,n);
//     #elif defined(__AVX__)
//     LNInt64Vector_Mul_AVX(z,a,b,n);
//     #elif defined(__SSE__)
//     LNInt64Vector_Mul_SSE(z,a,b,n);
//     #else
//     size_t i;
//     for(i=0;i<n;i++)
//         z[i]=a[i]*b[i];
//     #endif
//     return z;
// }

// float *LNFloat32Vector_Fill(float *v, float fill_value){
//     #if defined(__AVX2__)
//     LNFloat32Vector_Fill_AVX2(z,a,b,n);
//     #elif defined(__AVX__)
//     LNFloat32Vector_Fill_AVX(z,a,b,n);
//     #elif defined(__SSE__)
//     LNFloat32Vector_Fill_SSE(z,a,b,n);
//     #else
//     size_t i;
//     for(i=0;i<n;i++)
//         v[i]=fill_value;
//     #endif
//     return z;
// }

// float *LNFloat32Vector_Add(float *z, const float *a, const float *b, size_t n){
//     #if defined(__AVX2__)
//     LNFloat32Vector_Add_AVX2(z,a,b,n);
//     #elif defined(__AVX__)
//     LNFloat32Vector_Add_AVX(z,a,b,n);
//     #elif defined(__SSE__)
//     LNFloat32Vector_Add_SSE(z,a,b,n);
//     #else
//     size_t i;
//     for(i=0;i<n;i++)
//         z[i]=a[i]+b[i];
//     #endif
//     return z;
// }

// float *LNFloat32Vector_Sub(float *z, const float *a, const float *b, size_t n){
//     #if defined(__AVX2__)
//     LNFloat32Vector_Sub_AVX2(z,a,b,n);
//     #elif defined(__AVX__)
//     LNFloat32Vector_Sub_AVX(z,a,b,n);
//     #elif defined(__SSE__)
//     LNFloat32Vector_Sub_SSE(z,a,b,n);
//     #else
//     size_t i;
//     for(i=0;i<n;i++)
//         z[i]=a[i]-b[i];
//     #endif
//     return z;
// }

// float *LNFloat32Vector_Div(float *z, const float *a, const float *b, size_t n){
//     #if defined(__AVX2__)
//     LNFloat32Vector_Div_AVX2(z,a,b,n);
//     #elif defined(__AVX__)
//     LNFloat32Vector_Div_AVX(z,a,b,n);
//     #elif defined(__SSE__)
//     LNFloat32Vector_Div_SSE(z,a,b,n);
//     #else
//     size_t i;
//     for(i=0;i<n;i++)
//         z[i]=a[i]/b[i];
//     #endif
//     return z;
// }

// float *LNFloat32Vector_Mul(float *z, const float *a, const float *b, size_t n){
//     #if defined(__AVX2__)
//     LNFloat32Vector_Mul_AVX2(z,a,b,n);
//     #elif defined(__AVX__)
//     LNFloat32Vector_Mul_AVX(z,a,b,n);
//     #elif defined(__SSE__)
//     LNFloat32Vector_Mul_SSE(z,a,b,n);
//     #else
//     size_t i;
//     for(i=0;i<n;i++)
//         z[i]=a[i]*b[i];
//     #endif
//     return z;
// }

// double *LNFloat64Vector_Fill(double *v, double fill_value){
//     #if defined(__AVX2__)
//     LNFloat64Vector_Fill_AVX2(z,a,b,n);
//     #elif defined(__AVX__)
//     LNFloat64Vector_Fill_AVX(z,a,b,n);
//     #elif defined(__SSE__)
//     LNFloat64Vector_Fill_SSE(z,a,b,n);
//     #else
//     size_t i;
//     for(i=0;i<n;i++)
//         v[i]=fill_value;
//     #endif
//     return z;
// }

// double *LNFloat64Vector_Add(double *z, const double *a, const double *b, size_t n){
//     #if defined(__AVX2__)
//     LNFloat64Vector_Add_AVX2(z,a,b,n);
//     #elif defined(__AVX__)
//     LNFloat64Vector_Add_AVX(z,a,b,n);
//     #elif defined(__SSE__)
//     LNFloat64Vector_Add_SSE(z,a,b,n);
//     #else
//     size_t i;
//     for(i=0;i<n;i++)
//         z[i]=a[i]+b[i];
//     #endif
//     return z;
// }

// double *LNFloat64Vector_Sub(double *z, const double *a, const double *b, size_t n){
//     #if defined(__AVX2__)
//     LNFloat64Vector_Sub_AVX2(z,a,b,n);
//     #elif defined(__AVX__)
//     LNFloat64Vector_Sub_AVX(z,a,b,n);
//     #elif defined(__SSE__)
//     LNFloat64Vector_Sub_SSE(z,a,b,n);
//     #else
//     size_t i;
//     for(i=0;i<n;i++)
//         z[i]=a[i]-b[i];
//     #endif
//     return z;
// }

// double *LNFloat64Vector_Div(double *z, const double *a, const double *b, size_t n){
//     #if defined(__AVX2__)
//     LNFloat64Vector_Div_AVX2(z,a,b,n);
//     #elif defined(__AVX__)
//     LNFloat64Vector_Div_AVX(z,a,b,n);
//     #elif defined(__SSE__)
//     LNFloat64Vector_Div_SSE(z,a,b,n);
//     #else
//     size_t i;
//     for(i=0;i<n;i++)
//         z[i]=a[i]/b[i];
//     #endif
//     return z;
// }

// double *LNFloat64Vector_Mul(double *z, const double *a, const double *b, size_t n){
//     #if defined(__AVX2__)
//     LNFloat64Vector_Mul_AVX2(z,a,b,n);
//     #elif defined(__AVX__)
//     LNFloat64Vector_Mul_AVX(z,a,b,n);
//     #elif defined(__SSE__)
//     LNFloat64Vector_Mul_SSE(z,a,b,n);
//     #else
//     size_t i;
//     for(i=0;i<n;i++)
//         z[i]=a[i]*b[i];
//     #endif
//     return z;
// }

// #define LN_GENERATE_COMPLEX_OPERATOR(complex_type, Complex_type, op, Op)\
//     complex_type *LN##Complex_type##Vector_##Op(complex_type *z, const complex_type *a, const complex_type *b, size_t n){\
//         size_t i;\
//         for(i=0;i<n;i++){\
//             z[i].real = a[i].real op b[i].real;\
//             z[i].imag = a[i].imag op b[i].imag;\
//         }\
// \
//         return z;\
//     }

// LN_GENERATE_COMPLEX_OPERATOR(complex128_t, Complex128, +, Add)
// LN_GENERATE_COMPLEX_OPERATOR(complex128_t, Complex128, -, Sub)
// LN_GENERATE_COMPLEX_OPERATOR(complex128_t, Complex128, *, Mul)
// LN_GENERATE_COMPLEX_OPERATOR(complex128_t, Complex128, /, Div)

// LN_GENERATE_COMPLEX_OPERATOR(complex64_t, Complex64, +, Add)
// LN_GENERATE_COMPLEX_OPERATOR(complex64_t, Complex64, -, Sub)
// LN_GENERATE_COMPLEX_OPERATOR(complex64_t, Complex64, *, Mul)
// LN_GENERATE_COMPLEX_OPERATOR(complex64_t, Complex64, /, Div)

// complex128_t *LNComplex128Vector_Fill(complex128_t *v, complex128_t fill_value, size_t n){
//     size_t i;
//     #if defined(_OPENMP)
//     #pragma omp parallel for
//     #endif
//     for(i=0;i<n;i++){
//         v[i]=fill_value;
//     }
// }

// complex128_t LNComplex128Vector_Dot(const complex128_t *a, const complex128_t *b, size_t n){
//     complex128_t z = {0,0};
//     size_t i;
//     #if defined(_OPENMP)
//     #pragma omp parallel for
//     #endif
//     for(i=0;i<n;i++){
//         z.real += a[i].real * b[i].real;
//         z.imag += b[i].imag * b[i].imag;
//     }
//     return z;
// }

// complex64_t LNComplex64Vector_Dot(const complex64_t *a, const complex64_t *b, size_t n){
//     complex64_t z = {0,0};
//     size_t i;
//     for(i=0;i<n;i++){
//         z.real += a[i].real * b[i].real;
//         z.imag += b[i].imag * b[i].imag;
//     }
//     return z;
// }

// #undef LN_GENERATE_COMPLEX_OPERATOR

#include "utils.h"
#include "vector.h"

#define LNVector_(name) LN_CONCAT4(LN, Real, Vector_, name)

#include "generic/vector.c"
#include "LNGenerateInt8Type.h"

#include "generic/vector.c"
#include "LNGenerateInt16Type.h"

#include "generic/vector.c"
#include "LNGenerateInt32Type.h"

#include "generic/vector.c"
#include "LNGenerateInt64Type.h"

#include "generic/vector.c"
#include "LNGenerateFloat32Type.h"

#include "generic/vector.c"
#include "LNGenerateFloat64Type.h"

#include "generic/vector.c"
#include "LNGenerateComplex64Type.h"

#include "generic/vector.c"
#include "LNGenerateComplex128Type.h"

#undef LNVector_
