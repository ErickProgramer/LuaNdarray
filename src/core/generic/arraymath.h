#ifndef LN_GENERIC_FILE
#define LN_GENERIC_FILE "generic/arraymath.h"
#else

#include "../arraytypes.h"

Ndarray *LNMath_(SumAxis)(Ndarray *arr, long long axis);
Ndarray *LNMath_(MaxAxis)(Ndarray *arr, long long axis);
Ndarray *LNMath_(MinAxis)(Ndarray *arr, long long axis);

Ndarray *LNMath_(Sum)(Ndarray *arr);
Ndarray *LNMath_(Max)(Ndarray *arr);
Ndarray *LNMath_(Min)(Ndarray *arr);

#endif /* LUANDARRAY_SRC_CORE_GENERIC_ARRAYMATH_H_ */