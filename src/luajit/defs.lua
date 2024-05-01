-- LuaNdarray C-API functions definitions --

local ffi = require "ffi"

ffi.cdef[[
typedef float float32_t;
typedef double float64_t;

typedef struct complex128_t complex128_t;
typedef struct complex64_t complex64_t;

typedef enum LNDTypes LNDTypes;

typedef struct Ndarray_fields Ndarray;
typedef struct LNTypeDescr LNTypeDescr;

struct complex128_t{
    float64_t realp;
    float64_t imagp;
};

struct complex64_t{
    float32_t realp;
    float32_t imagp;
};

typedef unsigned char bool_t;
enum LNDTypes{
    LN_CHAR=0,
    LN_BOOL,

    LN_INT8,
    LN_INT16,
    LN_INT32,
    LN_INT64,

    LN_UINT8,
    LN_UINT16,
    LN_UINT32,
    LN_UINT64,

    LN_FLOAT32,
    LN_FLOAT64,

    LN_COMPLEX64,
    LN_COMPLEX128,

    LN_NUMTYPES
};

typedef void (*CastFunc)(void *dst, const void *src, LNDTypes newdtype, size_t n);

typedef void (*LNFillFunc)(void *vec, const void *v, size_t n);

typedef Ndarray *(*LNAxisOperator)(Ndarray *arr, long long axis);
typedef Ndarray *(*LNMapOperator)(Ndarray *arr);

struct LNTypeDescr{
    LNDTypes id;
    size_t alignment;
    size_t itemsize;

    char *name;
    char *fmt;

    CastFunc castfunc;
    LNAxisOperator max_axis_func;
    LNAxisOperator min_axis_func;
    LNAxisOperator sum_axis_func;

    LNMapOperator max_func;
    LNMapOperator min_func;
    LNMapOperator sum_func;
};

/* ndarray struct */
struct Ndarray_fields{
    /* data of the ndarray */
    char *data;

    /* dimensions of the array */
    size_t *dimensions;

    long long *strides;

    /* number of dimensions */
    size_t nd;

    /* size of `data` (multiplication of the dimensions) */
    size_t size;

    const LNTypeDescr *dtype;
};    

// types
const LNTypeDescr *LNInt8;
const LNTypeDescr *LNInt16;
const LNTypeDescr *LNInt32;
const LNTypeDescr *LNInt64;

const LNTypeDescr *LNUInt8;
const LNTypeDescr *LNUInt16;
const LNTypeDescr *LNUInt32;
const LNTypeDescr *LNUInt64;

const LNTypeDescr *LNFloat32;
const LNTypeDescr *LNFloat64;

const LNTypeDescr *LNComplex64;
const LNTypeDescr *LNComplex128;

const LNTypeDescr *LNChar;
const LNTypeDescr *LNBool;

typedef struct LNError_t LNError_t;

LNError_t LNExc_Error;

int LNError_Ocurred();
char *LNError_Get();

void *LNMem_alloc(size_t bytes);

void LNDType_Init();
void LNDType_Free(LNTypeDescr *dtype);

const LNTypeDescr *LNDType_Promote(const LNTypeDescr *t1, const LNTypeDescr *t2);

Ndarray *LNArray_New(void *data, size_t *dims, size_t nd, const LNTypeDescr *dtype);
Ndarray *LNArray_Zeros(size_t *dims, size_t nd, const LNTypeDescr *dtype);
Ndarray *LNArray_Ones(size_t *dims, size_t nd, const LNTypeDescr *dtype);

Ndarray *LNArray_Index(Ndarray *arr, long long idx);
void LNArray_Free(Ndarray *arr);

char *LNArray_toString(Ndarray *arr, const char *prefix, const char *sufix);

void LNArray_BroadcastTo(Ndarray *out, Ndarray *arr, size_t *to, size_t nd, const char *err);
Ndarray *LNArray_Add(Ndarray *arr1, Ndarray *arr2);
Ndarray *LNArray_CastTo(Ndarray *arr, const LNTypeDescr *newtype);

Ndarray *LNArray_Max(Ndarray *arr);
Ndarray *LNArray_MaxAxis(Ndarray *arr, long long axis);

Ndarray *LNArray_Empty(const size_t *dims, size_t ndim, const LNTypeDescr *dtype);
]]

---@class ln.dtype*
---@field id ffi.cdata*
---@field name ffi.cdata*
---@field alignment ffi.cdata*
---@field itemsize ffi.cdata*

---@class ln.dtype
---@overload fun(data: any): ln.Ndarray
---@operator call(ln.dtype): ln.dtype
---@field name string
---@field alignment integer
---@field itemsize integer
---@field dtype ln.dtype*
