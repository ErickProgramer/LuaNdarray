#ifndef LN_GENERIC_FILE
#define LN_GENERIC_FILE "generic/vector.c"
#else

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define LN_DEFAULT_VECTOR_OP(op)\
    for(i = 0; i < n; i++){\
        z[i] = a[i] op b[i];\
    }

#if defined(LN_REAL_IS_COMPLEX)

real *LNVector_(Fill)(real *v, real fill_value, size_t n){
    size_t i;
    for(i=0;i<n;i++){
        v[i]=fill_value;
    }
    return v;
}

real *LNVector_(Add)(real *z, const real *a, const real *b, size_t n){
    size_t i;
    for(i=0;i<n;i++){
        z[i].realp = a[i].realp + a[i].realp;
        z[i].imagp = b[i].imagp + b[i].imagp;
    }
    return z;
}

real *LNVector_(Sub)(real *z, const real *a, const real *b, size_t n){
    size_t i;
    for(i=0;i<n;i++){
        z[i].realp=a[i].realp-a[i].realp;
        z[i].imagp=b[i].imagp-b[i].imagp;
    }
    return z;
}

real *LNVector_(Mul)(real *z, const real *a, const real *b, size_t n){
    size_t i;
    for(i=0;i<n;i++){
        z[i].realp=a[i].realp*b[i].realp-a[i].imagp*b[i].imagp;
        z[i].imagp=a[i].realp*b[i].imagp+b[i].realp*a[i].imagp;
    }
    return z;
}

#define rect(r, phi) {.realp=(r)*cos(phi), .imagp=(r)*sin(phi)}

real *LNVector_(Div)(real *z, const real *a, const real *b, size_t n){
    #if defined(LN_REAL_IS_COMPLEX64)
    float32_t rsq, phi;
    #else
    float64_t rsq, phi;
    #endif
    size_t i;
    for(i=0;i<n;i++){
        rsq=pow(a[i].realp,2)+pow(a[i].imagp,2);
        phi=atan2(a[i].imagp, a[i].realp);
        z[i]=(real)rect(pow(rsq, b[i].realp/2)*exp(b[i].imagp), b[i].imagp*log(rsq)/2+b[i].realp*phi);
    }
    return z;
}

real *LNVector_(Range)(real *z, double start, double stop, double step){
    size_t p=0;
    double i;
    for(i=start; i<stop; i+=step, p++){
        z[p].realp = i;
        z[p].imagp = 0;
    }
    return z;
}

#else

#if defined(__AVX512F__)
#include"../simd/AVX512.h"
#include"../simd/AVX.h"
#include"../simd/SSE.h"
#elif defined(__AVX__)
#include"../simd/AVX.h"
#include"../simd/SSE.h"
#elif defined(__SSE__)
#include"../simd/SSE.h"
#endif

real *LNVector_(Fill)(real *v, real fill_value, size_t n){
    #if defined(__AVX512F__) && defined(LN_REAL_IS_FLOAT)
        LNVector_(Fill_AVX512)(v, fill_value, n);
    #elif defined(__AVX__) && defined(LN_REAL_IS_FLOAT32)
        LNVector_(Fill_AVX)(v, fill_value, n);
    #elif defined(__AVX2__) && defined(LN_REAL_IS_FLOAT64)
        LNVector_(Fill_AVX)(v, fill_value, n);
    #elif defined(__SSE__) && defined(LN_REAL_IS_FLOAT32)
        LNVector_(Fill_SSE)(v, fill_value, n);
    #elif defined(__SSE2__) && defined(LN_REAL_IS_FLOAT64)
        LNVector_(Fill_SSE)(v, fill_value, n);
    #else
        size_t i;
        for(i = 0; i < n; i++){
            v[i] = fill_value;
        }
    #endif
    return v;
}

real *LNVector_(Add)(real *z, const real *a, const real *b, size_t n){
    #if defined(__AVX512F__) && defined(LN_REAL_IS_FLOAT)
        LNVector_(Add_AVX512)(z, a, b, n);
    #elif defined(__AVX__) && defined(LN_REAL_IS_FLOAT32)
        LNVector_(Add_AVX)(z, a, b, n);
    #elif defined(__AVX2__) && defined(LN_REAL_IS_FLOAT64)
        LNVector_(Add_AVX)(z, a, b, n);
    #elif defined(__SSE__) && defined(LN_REAL_IS_FLOAT32)
        LNVector_(Add_SSE)(z, a, b, n);
    #elif defined(__SSE2__) && defined(LN_REAL_IS_FLOAT64)
        LNVector_(Add_SSE)(z, a, b, n);
    #else
        size_t i;
        for(i = 0; i < n; i++){
            z[i] = a[i] + b[i];
        }
    #endif
    return z;
}

real *LNVector_(Sub)(real *z, const real *a, const real *b, size_t n){
    #if defined(__AVX512F__) && defined(LN_REAL_IS_FLOAT)
        LNVector_(Sub_AVX512)(z, a, b, n);
    #elif defined(__AVX__) && defined(LN_REAL_IS_FLOAT32)
        LNVector_(Sub_AVX)(z, a, b, n);
    #elif defined(__AVX2__) && defined(LN_REAL_IS_FLOAT64)
        LNVector_(Sub_AVX)(z, a, b, n);
    #elif defined(__SSE__) && defined(LN_REAL_IS_FLOAT32)
        LNVector_(Sub_SSE)(z, a, b, n);
    #elif defined(__SSE2__) && defined(LN_REAL_IS_FLOAT64)
        LNVector_(Sub_SSE)(z, a, b, n);
    #else
        size_t i;
        for(i = 0; i < n; i++){
            z[i] = a[i] - b[i];
        }
    #endif
    return z;
}

real *LNVector_(Mul)(real *z, const real *a, const real *b, size_t n){
    #if defined(__AVX512F__) && defined(LN_REAL_IS_FLOAT)
        LNVector_(Mul_AVX512)(z, a, b, n);
    #elif defined(__AVX__) && defined(LN_REAL_IS_FLOAT32)
        LNVector_(Mul_AVX)(z, a, b, n);
    #elif defined(__AVX2__) && defined(LN_REAL_IS_FLOAT64)
        LNVector_(Mul_AVX)(z, a, b, n);
    #elif defined(__SSE__) && defined(LN_REAL_IS_FLOAT32)
        LNVector_(Mul_SSE)(z, a, b, n);
    #elif defined(__SSE2__) && defined(LN_REAL_IS_FLOAT64)
        LNVector_(Mul_SSE)(z, a, b, n);
    #else
        size_t i;
        for(i = 0; i < n; i++){
            z[i] = a[i] * b[i];
        }
    #endif
    return z;
}

real *LNVector_(Div)(real *z, const real *a, const real *b, size_t n){
    #if defined(__AVX512F__) && defined(LN_REAL_IS_FLOAT)
        LNVector_(Div_AVX512)(z, a, b, n);
    #elif defined(__AVX__) && defined(LN_REAL_IS_FLOAT32)
        LNVector_(Div_AVX)(z, a, b, n);
    #elif defined(__AVX2__) && defined(LN_REAL_IS_FLOAT64)
        LNVector_(Div_AVX)(z, a, b, n);
    #elif defined(__SSE__) && defined(LN_REAL_IS_FLOAT32)
        LNVector_(Div_SSE)(z, a, b, n);
    #elif defined(__SSE2__) && defined(LN_REAL_IS_FLOAT64)
        LNVector_(Div_SSE)(z, a, b, n);
    #else
        size_t i;
        for(i = 0; i < n; i++){
            z[i] = a[i] / b[i];
        }
    #endif
    return z;
}

#if defined(LN_REAL_IS_INTEGER)
real *LNVector_(Range)(real *z, long long start, long long stop, long long step)
#else
real *LNVector_(Range)(real *z, double start, double stop, double step)
#endif
{
    size_t p=0;
    double i;
    for(i=start; i<stop; i+=step, p++){
        z[p] = i;
    }
    return z;
}

#endif


#endif
