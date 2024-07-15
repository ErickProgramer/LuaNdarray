#ifndef LN_GENERIC_FILE
#error "LN_GENERIC_FILE not defined"
#endif

#define LN_GENERATE_MANY_TYPES

#include "LNGenerateInt8Type.h"
#include "LNGenerateInt16Type.h"
#include "LNGenerateInt32Type.h"
#include "LNGenerateInt64Type.h"

#include "LNGenerateUInt8Type.h"
#include "LNGenerateUInt16Type.h"
#include "LNGenerateUInt32Type.h"
#include "LNGenerateUInt64Type.h"

#include "LNGenerateFloat32Type.h"
#include "LNGenerateFloat64Type.h"

#include "LNGenerateComplex64Type.h"
#include "LNGenerateComplex128Type.h"

#undef LN_GENERIC_FILE
#undef LN_GENERATE_MANY_TYPES
