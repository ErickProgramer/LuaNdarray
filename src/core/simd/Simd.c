#ifndef LN_GENERIC_FILE
#define LN_GENERIC_FILE "simd/Simd.c"
#else

#include<stdio.h>
#include<stdlib.h>
#include<emmintrin.h>

real *LNSimd_(Fill)(real *v, real fill_value, size_t n){
    LNSimdVec vec = LNSimd(set1)(fill_value);

    size_t i;
    for(i = 0; i < ((n)-(n%LN_LOOP_STEP)); i+=LN_LOOP_STEP){
        #if defined(LN_REAL_IS_INTEGER)
        LNSimd(storeu)((__m128i*)(v+i), vec);
        #else
        LNSimd(storeu)(v+i, vec);
        #endif
    }

    for(;i < n; i++){
        v[i] = fill_value;
    }

    return v;
}

real *LNSimd_(Add)(real *z, const real *a, const real *b, size_t n){
    LNSimdVec res;
    LNSimdVec _a;
    LNSimdVec _b;

    size_t i;
    for(i = 0; i < ((n)-(n%LN_LOOP_STEP)); i+=LN_LOOP_STEP){
        _a = LNSimd(loadu)(a + i);
        _b = LNSimd(loadu)(b + i);
        res = LNSimd(add)(_a, _b);
        #if defined(LN_REAL_IS_INTEGER)
        LNSimd(storeu)((__m128i*)(z+i), res);
        #else
        LNSimd(storeu)(z+i, res);
        #endif
    }

    for(; i < n; i++){
        z[i] = a[i]+b[i];
    }

    return z;
}

real *LNSimd_(Sub)(real *z, const real *a, const real *b, size_t n){
    LNSimdVec res;
    LNSimdVec _a;
    LNSimdVec _b;

    size_t i;
    for(i = 0; i < ((n)-(n%LN_LOOP_STEP)); i+=LN_LOOP_STEP){
        _a = LNSimd(loadu)(a + i);
        _b = LNSimd(loadu)(b + i);
        res = LNSimd(sub)(_a, _b);
        #if defined(LN_REAL_IS_INTEGER)
        LNSimd(storeu)((__m128i*)(z+i), res);
        #else
        LNSimd(storeu)(z+i, res);
        #endif
    }

    for(; i < n; i++){
        z[i] = a[i]-b[i];
    }

    return z;
}

real *LNSimd_(Mul)(real *z, const real *a, const real *b, size_t n){
    LNSimdVec res;
    LNSimdVec _a;
    LNSimdVec _b;

    size_t i;
    for(i = 0; i < ((n)-(n%LN_LOOP_STEP)); i+=LN_LOOP_STEP){
        _a = LNSimd(loadu)(a + i);
        _b = LNSimd(loadu)(b + i);
        res = LNSimd(mul)(_a, _b);
        #if defined(LN_REAL_IS_INTEGER)
        LNSimd(storeu)((__m128i*)(z+i), res);
        #else
        LNSimd(storeu)(z+i, res);
        #endif
    }

    for(; i < n; i++){
        z[i] = a[i]*b[i];
    }

    return z;
}

real *LNSimd_(Div)(real *z, const real *a, const real *b, size_t n){
    LNSimdVec res;
    LNSimdVec _a;
    LNSimdVec _b;

    size_t i;
    for(i = 0; i < ((n)-(n%LN_LOOP_STEP)); i+=LN_LOOP_STEP){
        _a = LNSimd(loadu)(a + i);
        _b = LNSimd(loadu)(b + i);
        res = LNSimd(div)(_a, _b);
        #if defined(LN_REAL_IS_INTEGER)
        LNSimd(storeu)((__m128i*)(z+i), res);
        #else
        LNSimd(storeu)(z+i, res);
        #endif
    }

    for(; i < n; i++){
        z[i] = a[i]/b[i];
    }

    return z;
}



#endif