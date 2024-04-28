#ifndef LN_GENERIC_FILE
#define LN_GENERIC_FILE "simd/Simd.h"
#else

#include<stdio.h>
#include<stdlib.h>

real *LNSimd_(Fill)(real *v, real fill_value);
real *LNSimd_(Add)(real *z, const real *a, const real *b, size_t n);
real *LNSimd_(Sub)(real *z, const real *a, const real *b, size_t n);
real *LNSimd_(Mul)(real *z, const real *a, const real *b, size_t n);
real *LNSimd_(Div)(real *z, const real *a, const real *b, size_t n);

#endif