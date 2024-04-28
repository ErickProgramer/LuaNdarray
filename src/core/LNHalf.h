#ifndef LUANDARRAY_SRC_CORE_LNHALF_H_
#define LUANDARRAY_SRC_CORE_LNHALF_H_

#include<stdint.h>

typedef int16_t LNHalf_t;

LNHalf_t LN_float2half(float x);
float LN_half2float(LNHalf_t x);

#endif