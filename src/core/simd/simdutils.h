#ifndef LUANDARRAY_CORE_SIMD_SIMDUTILS_H_

#include<emmintrin.h>

#define _mm_loadu_epi8(p) _mm_loadu_si128((__m128i*)((int8_t*)p))
#define _mm_loadu_epi16(p) _mm_loadu_si128((__m128i*)((int16_t*)p))
#define _mm_loadu_epi32(p) _mm_loadu_si128((__m128i*)((int32_t*)p))
#define _mm_loadu_epi64(p) _mm_loadu_si128((__m128i*)((int64_t*)p))

#define _mm_storeu_epi8(p,b) _mm_storeu_si128((__m128i*)((int8_t*)p),b)
#define _mm_storeu_epi16(p,b) _mm_storeu_si128((__m128i*)((int16_t*)p),b)
#define _mm_storeu_epi32(p,b) _mm_storeu_si128((__m128i*)((int32_t*)p),b)
#define _mm_storeu_epi64(p,b) _mm_storeu_si128((__m128i*)((int64_t*)p),b)

#define _mm_storeu_epu8(p,b) _mm_storeu_si128((__m128i*)((uint8_t*)p),b)
#define _mm_storeu_epu16(p,b) _mm_storeu_si128((__m128i*)((uint16_t*)p),b)
#define _mm_storeu_epu32(p,b) _mm_storeu_si128((__m128i*)((uint32_t*)p),b)
#define _mm_storeu_epu64(p,b) _mm_storeu_si128((__m128i*)((uint64_t*)p),b)

#define _mm_set1_epu8 _mm_set1_epi8
#define _mm_set1_epu16 _mm_set1_epi16
#define _mm_set1_epu32 _mm_set1_epi32
#define _mm_set1_epu64 _mm_set1_epi64

#define _mm_mul_epi16 _mm_mullo_epi16
#define _mm_mul_epi32(a,b) (__m128i)_mm_mul_ps((__m128)(a),(__m128)(b))
#define _mm_mul_epi64(a,b) (__m128i)_mm_mul_pd((__m128d)(a),(__m128d)(b))

#define _mm_div_epi32(a,b) (__m128i)_mm_mul_ps((__m128)(a),(__m128)(b))
#define _mm_div_epi64(a,b) (__m128i)_mm_mul_pd((__m128d)(a),(__m128d)(b))

static inline __m128 _mm_mul_epi8(__m128i X)
{
    __m128i zero = _mm_setzero_si128();
    __m128i sign_x = _mm_cmplt_epi8(X, zero);
    __m128i xlo = _mm_unpacklo_epi8(X, sign_x);
    __m128i xhi = _mm_unpackhi_epi8(X, sign_x);

    return _mm_cvtepi32_ps(_mm_add_epi32(_mm_madd_epi16(xlo, xlo), _mm_madd_epi16(xhi, xhi)));
}

#define _mm256_mul_epi16 _mm256_mullo_epi16


#define _mm256_loadu_epi8(p) _mm256_loadu_si256((__m256i*)((int8_t*)p))
#define _mm256_loadu_epi16(p) _mm256_loadu_si256((__m256i*)((int16_t*)p))
#define _mm256_loadu_epi32(p) _mm256_loadu_si256((__m256i*)((int32_t*)p))
#define _mm256_loadu_epi64(p) _mm256_loadu_si256((__m256i*)((int64_t*)p))

#define _mm256_storeu_epi8(p,b) _mm256_storeu_si256((__m256i*)((int8_t*)p),b)
#define _mm256_storeu_epi16(p,b) _mm256_storeu_si256((__m256i*)((int16_t*)p),b)
#define _mm256_storeu_epi32(p,b) _mm256_storeu_si256((__m256i*)((int32_t*)p),b)
#define _mm256_storeu_epi64(p,b) _mm256_storeu_si256((__m256i*)((int64_t*)p),b)

#define _mm256_storeu_epu8(p,b) _mm256_storeu_si256((__m256i*)((uint8_t*)p),b)
#define _mm256_storeu_epu16(p,b) _mm256_storeu_si256((__m256i*)((uint16_t*)p),b)
#define _mm256_storeu_epu32(p,b) _mm256_storeu_si256((__m256i*)((uint32_t*)p),b)
#define _mm256_storeu_epu64(p,b) _mm256_storeu_si256((__m256i*)((uint64_t*)p),b)


#define _mm256_div_epi32(a,b) (__m256i)_mm256_div_ps((__m256)(a),(__m256)(b))
#define _mm256_div_epi64(a,b) (__m256i)_mm256_div_pd((__m256d)(a),(__m256d)(b))

#define _mm256_mul_epi64(a,b) (__m256i)_mm256_mul_pd((__m256d)a,(__m256d)b);

#endif