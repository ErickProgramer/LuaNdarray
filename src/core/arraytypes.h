#ifndef LUANDARRAY_SRC_CORE_ARRAYTYPES_H_
#define LUANDARRAY_SRC_CORE_ARRAYTYPES_H_

#include<stdio.h>
#include<stdlib.h>

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

#define ln_c64real(v) (*(complex64_t*)(v)).realp
#define ln_c64imag(v) (*(complex64_t*)(v)).imagp

#define ln_c128real(v) (*(complex128_t*)(v)).realp
#define ln_c128imag(v) (*(complex128_t*)(v)).imagp

typedef unsigned char bool_t;

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif


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

// typedef void (*CastFunc)(void *dst, const void *src, LNDTypes newdtype, size_t n);
typedef void (*CastFunc)(Ndarray *dst, const Ndarray *src, LNDTypes newtype);

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



#endif