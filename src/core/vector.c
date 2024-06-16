#include "utils.h"
#include "vector.h"

#define LNVector_(name) LN_CONCAT4(LN, Real, Vector_, name)

#include "generic/vector.c"
#include "LNGenerateInt8Type.h"

#include "generic/vector.c"
#include "LNGenerateInt16Type.h"

#include "generic/vector.c"
#include "LNGenerateInt32Type.h"

#include "generic/vector.c"
#include "LNGenerateInt64Type.h"

#include "generic/vector.c"
#include "LNGenerateFloat32Type.h"

#include "generic/vector.c"
#include "LNGenerateFloat64Type.h"

#include "generic/vector.c"
#include "LNGenerateComplex64Type.h"

#include "generic/vector.c"
#include "LNGenerateComplex128Type.h"

#undef LNVector_
