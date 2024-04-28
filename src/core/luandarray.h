#ifndef LUANDARRAY_H_
#define LUANDARRAY_H_

#define require_luandarray()\
    do{\
        LNDType_Init();\
        LNError_Init();\
        if(LNError_Ocurred()){\
            fprintf(stderr, "Fatal Error on initialize LuaNdarray. "\
                            "Last error message: %s", LNError_Get());\
            return 1;\
        }\
    }while(0)\

#define LN_Finalize()\
    do{\
        LNDType_Free((LNTypeDescr*)LNInt8);\
        LNDType_Free((LNTypeDescr*)LNInt16);\
        LNDType_Free((LNTypeDescr*)LNInt32);\
        LNDType_Free((LNTypeDescr*)LNInt64);\
\
        LNDType_Free((LNTypeDescr*)LNUInt8);\
        LNDType_Free((LNTypeDescr*)LNUInt16);\
        LNDType_Free((LNTypeDescr*)LNUInt32);\
        LNDType_Free((LNTypeDescr*)LNUInt64);\
\
        LNDType_Free((LNTypeDescr*)LNFloat32);\
        LNDType_Free((LNTypeDescr*)LNFloat64);\
    }while(0)

#include "error.h"
#include "buffer.h"
#include "utils.h"
#include "arrayobj.h"
#include "arrayindex.h"
#include "arraydims.h"
#include "converts.h"
#include "arraydtype.h"
#include "arrayinis.h"
#include "arraycast.h"
#include "arraymath.h"

#endif