#ifndef LN_GENERIC_FILE
#define LN_GENERIC_FILE "generic/vector.h"
#else

real *LNVector_(Fill)(real *v, real fill_value, size_t n);
real *LNVector_(Add)(real *z, const real *a, const real *b, size_t n);
real *LNVector_(Sub)(real *z, const real *a, const real *b, size_t n);
real *LNVector_(Mul)(real *z, const real *a, const real *b, size_t n);
real *LNVector_(Div)(real *z, const real *a, const real *b, size_t n);
#if defined(LN_REAL_IS_INTEGER)
real *LNVector_(Range)(real *z, long long start, long long stop, long long step);
#else
real *LNVector_(Range)(real *z, double start, double stop, double step);
#endif

#endif