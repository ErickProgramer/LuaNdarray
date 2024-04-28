#ifndef LN_GENERIC_FILE
#error "define LN_GENERIC_FILE."
#endif

#include<stdint.h>

#include "arraydtype.h"

#define real int32_t
#define Real Int32
#define LN_REAL_IS_INT32
#define LN_REAL_IS_INTEGER
#line 1 LN_GENERIC_FILE
#include LN_GENERIC_FILE
#undef real
#undef Real
#undef LN_REAL_IS_INT32
#undef LN_REAL_IS_INTEGER
#ifndef LN_GENERATE_MANY_TYPES
#undef LN_GENERIC_FILE
#endif
