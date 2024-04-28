#ifndef LN_GENERIC_FILE
#error "define LN_GENERIC_FILE."
#endif

#include<stdint.h>

#include "arraydtype.h"

#define real complex128_t
#define Real Complex128
#define LN_REAL_IS_COMPLEX128
#define LN_REAL_IS_COMPLEX
#line 1 LN_GENERIC_FILE
#include LN_GENERIC_FILE
#undef real
#undef Real
#undef LN_REAL_IS_COMPLEX128
#undef LN_REAL_IS_COMPLEX
#ifndef LN_GENERATE_MANY_TYPES
#undef LN_GENERIC_FILE
#endif
