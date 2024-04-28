#include <stdio.h>
#include <stdbool.h>

#ifdef _MSC_VER
#include <intrin.h>
#endif

#ifdef __GNUC__

void __cpuid(int* cpuinfo, int info)
{
    __asm__ __volatile__(
        "xchg %%ebx, %%edi;"
        "cpuid;"
        "xchg %%ebx, %%edi;"
        :"=a" (cpuinfo[0]), "=D" (cpuinfo[1]), "=c" (cpuinfo[2]), "=d" (cpuinfo[3])
        :"0" (info)
    );
}

unsigned long long _xgetbv(unsigned int index)
{
    unsigned int eax, edx;
    __asm__ __volatile__(
        "xgetbv;"
        : "=a" (eax), "=d"(edx)
        : "c" (index)
    );
    return ((unsigned long long)edx << 32) | eax;
}

#endif

int main(){
    bool sseSupportted = false;
    bool sse2Supportted = false;
    bool sse3Supportted = false;
    bool ssse3Supportted = false;
    bool sse4_1Supportted = false;
    bool sse4_2Supportted = false;
    bool sse4aSupportted = false;
    bool sse5Supportted = false;
    bool avxSupportted = false;
    bool avx2Supportted = false;
    bool avx512Supportted = false;

    int cpuinfo[4];
    __cpuid(cpuinfo, 1);

    // Check SSE, SSE2, SSE3, SSSE3, SSE4.1, and SSE4.2 support
    sseSupportted        = cpuinfo[3] & (1 << 25) || false;
    sse2Supportted       = cpuinfo[3] & (1 << 26) || false;
    sse3Supportted       = cpuinfo[2] & (1 << 0) || false;
    ssse3Supportted      = cpuinfo[2] & (1 << 9) || false;
    sse4_1Supportted     = cpuinfo[2] & (1 << 19) || false;
    sse4_2Supportted     = cpuinfo[2] & (1 << 20) || false;

    // Check AVX support
    avxSupportted = cpuinfo[2] & (1 << 28) || false;
    bool osxsaveSupported = cpuinfo[2] & (1 << 27) || false;
    if (osxsaveSupported && avxSupportted)
    {
        // _XCR_XFEATURE_ENABLED_MASK = 0
        unsigned long long xcrFeatureMask = _xgetbv(0);
        avxSupportted = (xcrFeatureMask & 0x6) == 0x6;
    }

    // Check AVX2 support
    __cpuid(cpuinfo, 7);
    avx2Supportted = cpuinfo[1] & (1 << 5) || false;

    // Check AVX-512 support
    __cpuid(cpuinfo, 0);
    int numExtendedIds = cpuinfo[0];
    if (numExtendedIds >= 0xD)
    {
        __cpuid(cpuinfo, 0xD);
        avx512Supportted = cpuinfo[1] & (1 << 16) || false;
    }

    if(avx512Supportted){
        printf("avx512f");
    } else if(avxSupportted){
        printf("avx");
    } else if(sse4_2Supportted){
        printf("sse4.2");
    } else if(sse4_1Supportted){
        printf("sse4.1");
    } else if(sse3Supportted){
        printf("sse3");
    } else if(sse2Supportted){
        printf("sse2");
    } else if(sseSupportted){
        printf("sse");
    }

    return 0;
}
