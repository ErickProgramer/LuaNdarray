#ifndef LN_GENERIC_FILE
#define LN_GENERIC_FILE "generic/vector.c"
#else

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

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
/*
__mul=function(c1,c2)
		local r1,i1,r2,i2=re(c1),im(c1),re(c2),im(c2)
		return complex(r1*r2-i1*i2,r1*i2+r2*i1)
	end,
	__div=function(c1,c2)
		local r1,i1,r2,i2=re(c1),im(c1),re(c2),im(c2)
		local rsq=r2^2+i2^2
		return complex((r1*r2+i1*i2)/rsq,(r2*i1-r1*i2)/rsq)
	end,
	__pow=function(c1,c2)--Aww ye
		local r1,i1,r2,i2=re(c1),im(c1),re(c2),im(c2)
		local rsq=r1^2+i1^2
		if rsq==0 then--Things work better like this.
			if r2==0 and i2==0 then
				return 1
			end
			return 0
		end
		local phi=atan2(i1,r1)
		return rect(rsq^(r2/2)*exp(-i2*phi),i2*log(rsq)/2+r2*phi)
	end,
*/
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

#if defined(__AVX2__)
#include"../simd/AVX2.h"
#elif defined(__AVX__)
#include"../simd/AVX.h"
#elif defined(__SSE__)
#include"../simd/SSE.h"
#endif

real *LNVector_(Fill)(real *v, real fill_value, size_t n){
    #if defined(__AVX2__)
    LNVector_(Fill_AVX2)(v,fill_value,n);
    #elif defined(__AVX__)
    LNVector_(Fill_AVX)(v,fill_value,n);
    #elif defined(__SSE__)
    LNVector_(Fill_SSE)(v,fill_value,n);
    #else
    size_t i;
    for(i=0;i<n;i++){
        z[i]=fill_value;
    }
    #endif
}

real *LNVector_(Add)(real *z, const real *a, const real *b, size_t n){
    #if defined(__AVX2__)
    LNVector_(Add_AVX2)(z,a,b,n);
    #elif defined(__AVX__)
    LNVector_(Add_AVX)(z,a,b,n);
    #elif defined(__SSE__)
    LNVector_(Add_SSE)(z,a,b,n);
    #else
    size_t i;
    for(i=0;i<n;i++){
        z[i]=a[i]+b[i];
    }
    #endif
}

real *LNVector_(Sub)(real *z, const real *a, const real *b, size_t n){
    #if defined(__AVX2__)
    LNVector_(Sub_AVX2)(z,a,b,n);
    #elif defined(__AVX__)
    LNVector_(Sub_AVX)(z,a,b,n);
    #elif defined(__SSE__)
    LNVector_(Sub_SSE)(z,a,b,n);
    #else
    size_t i;
    for(i=0;i<n;i++){
        z[i]=a[i]-b[i];
    }
    #endif
    return z;
}

real *LNVector_(Mul)(real *z, const real *a, const real *b, size_t n){
    #if defined(LN_REAL_IS_INT8) || defined(LN_REAL_IS_INT16)
    size_t i;
    for(i=0;i<n;i++){
        z[i]=a[i]*b[i];
    }
    #else
    #if defined(__AVX2__)
    LNVector_(Mul_AVX2)(z,a,b,n);
    #elif defined(__AVX__)
    LNVector_(Mul_AVX)(z,a,b,n);
    #elif defined(__SSE__)
    LNVector_(Mul_SSE)(z,a,b,n);
    #else
    size_t i;
    for(i=0;i<n;i++){
        z[i]=a[i]*b[i];
    }
    #endif
    #endif
}

real *LNVector_(Div)(real *z, const real *a, const real *b, size_t n){
    #if defined(LN_REAL_IS_INT8) || defined(LN_REAL_IS_INT16)
    size_t i;
    for(i=0;i<n;i++){
        z[i]=a[i]*b[i];
    }
    #else
    #if defined(__AVX2__)
    LNVector_(Div_AVX2)(z,a,b,n);
    #elif defined(__AVX__)
    LNVector_(Div_AVX)(z,a,b,n);
    #elif defined(__SSE__)
    LNVector_(Div_SSE)(z,a,b,n);
    #else
    size_t i;
    for(i=0;i<n;i++){
        z[i]=a[i]/b[i];
    }
    #endif
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
