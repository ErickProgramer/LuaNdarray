#ifndef LUANDARRAY_MULTIARRAY_VECTOR_H_
#define LUANDARRAY_MULTIARRAY_VECTOR_H_

#include "utils.h"

#define LNVector_(name) LN_CONCAT4(LN, Real, Vector_, name)

#include "generic/vector.h"
#include "LNGenerateInt8Type.h"

#include "generic/vector.h"
#include "LNGenerateInt16Type.h"

#include "generic/vector.h"
#include "LNGenerateInt32Type.h"

#include "generic/vector.h"
#include "LNGenerateInt64Type.h"

#include "generic/vector.h"
#include "LNGenerateFloat32Type.h"

#include "generic/vector.h"
#include "LNGenerateFloat64Type.h"

#include "generic/vector.h"
#include "LNGenerateComplex64Type.h"

#include "generic/vector.h"
#include "LNGenerateComplex128Type.h"

#undef LNVector_

#endif