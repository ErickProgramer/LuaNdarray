#ifndef LUANDARRAY_MULTIARRAY_SIMD_AVX_H_
#define LUANDARRAY_MULTIARRAY_SIMD_AVX_H_


int8_t *LNInt8Vector_Fill_AVX(int8_t *v, int8_t fill_value, size_t n);
int8_t *LNInt8Vector_Add_AVX(int8_t *z, const int8_t *a, const int8_t *b, size_t n);
int8_t *LNInt8Vector_Sub_AVX(int8_t *z, const int8_t *a, const int8_t *b, size_t n);
int8_t *LNInt8Vector_Mul_AVX(int8_t *z, const int8_t *a, const int8_t *b, size_t n);
int8_t *LNInt8Vector_Div_AVX(int8_t *z, const int8_t *a, const int8_t *b, size_t n);

int16_t *LNInt16Vector_Fill_AVX(int16_t *v, int16_t fill_value, size_t n);
int16_t *LNInt16Vector_Add_AVX(int16_t *z, const int16_t *a, const int16_t *b, size_t n);
int16_t *LNInt16Vector_Sub_AVX(int16_t *z, const int16_t *a, const int16_t *b, size_t n);
int16_t *LNInt16Vector_Mul_AVX(int16_t *z, const int16_t *a, const int16_t *b, size_t n);
int16_t *LNInt16Vector_Div_AVX(int16_t *z, const int16_t *a, const int16_t *b, size_t n);

int32_t *LNInt32Vector_Fill_AVX(int32_t *v, int32_t fill_value, size_t n);
int32_t *LNInt32Vector_Add_AVX(int32_t *z, const int32_t *a, const int32_t *b, size_t n);
int32_t *LNInt32Vector_Sub_AVX(int32_t *z, const int32_t *a, const int32_t *b, size_t n);
int32_t *LNInt32Vector_Mul_AVX(int32_t *z, const int32_t *a, const int32_t *b, size_t n);
int32_t *LNInt32Vector_Div_AVX(int32_t *z, const int32_t *a, const int32_t *b, size_t n);

int64_t *LNInt64Vector_Fill_AVX(int64_t *v, int64_t fill_value, size_t n);
int64_t *LNInt64Vector_Add_AVX(int64_t *z, const int64_t *a, const int64_t *b, size_t n);
int64_t *LNInt64Vector_Sub_AVX(int64_t *z, const int64_t *a, const int64_t *b, size_t n);
int64_t *LNInt64Vector_Mul_AVX(int64_t *z, const int64_t *a, const int64_t *b, size_t n);
int64_t *LNInt64Vector_Div_AVX(int64_t *z, const int64_t *a, const int64_t *b, size_t n);

float *LNFloat32Vector_Fill_AVX(float *v, float fill_value, size_t n);
float *LNFloat32Vector_Add_AVX(float *z, const float *a, const float *b, size_t n);
float *LNFloat32Vector_Sub_AVX(float *z, const float *a, const float *b, size_t n);
float *LNFloat32Vector_Mul_AVX(float *z, const float *a, const float *b, size_t n);
float *LNFloat32Vector_Div_AVX(float *z, const float *a, const float *b, size_t n);

double *LNFloat64Vector_Fill_AVX(double *v, double fill_value, size_t n);
double *LNFloat64Vector_Add_AVX(double *z, const double *a, const double *b, size_t n);
double *LNFloat64Vector_Sub_AVX(double *z, const double *a, const double *b, size_t n);
double *LNFloat64Vector_Mul_AVX(double *z, const double *a, const double *b, size_t n);
double *LNFloat64Vector_Div_AVX(double *z, const double *a, const double *b, size_t n);

#endif /* LUANDARRAY_MULTIARRAY_SIMD_AVX_H_ */