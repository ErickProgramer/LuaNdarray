#ifndef LN_GENERIC_FILE
#error "define LN_GENERIC_FILE."
#endif

#include<stdint.h>

#include "arraydtype.h"

#define real float64_t
#define Real Float64
#define LN_REAL_IS_FLOAT64
#define LN_REAL_IS_FLOAT
#line 1 LN_GENERIC_FILE
#include LN_GENERIC_FILE
#undef real
#undef Real
#undef LN_REAL_IS_FLOAT64
#undef LN_REAL_IS_FLOAT
#ifndef LN_GENERATE_MANY_TYPES
#undef LN_GENERIC_FILE
#endif
