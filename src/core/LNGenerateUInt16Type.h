#ifndef LN_GENERIC_FILE
#error "define LN_GENERIC_FILE."
#endif

#include<stdint.h>

#include "arraydtype.h"

#define real uint16_t
#define Real UInt16
#define RealDType LNDType_GetFromID(LN_UINT16)
#define LN_REAL_IS_UINT16
#define LN_REAL_IS_INTEGER
#line 1 LN_GENERIC_FILE
#include LN_GENERIC_FILE
#undef real
#undef Real
#undef RealDType
#undef LN_REAL_IS_UINT16
#undef LN_REAL_IS_INTEGER
#ifndef LN_GENERATE_MANY_TYPES
#undef LN_GENERIC_FILE
#endif
