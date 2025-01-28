#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#include "arraydtype.h"
#include "error.h"
#include "arrayobj.h"
#include "arrayinis.h"
#include "vector.h"

Ndarray *LNArray_Empty(const size_t *dims, size_t ndim, const LNTypeDescr *dtype){
    Ndarray *arr = LNArray_Alloc();
    if(!arr)
        return NULL;

    arr->dimensions = LNMem_alloc(sizeof(size_t)*ndim);
    if(!arr->dimensions)return NULL;
    
    arr->strides = LNMem_alloc(sizeof(long long)*ndim);
    if(!arr->strides)
        return NULL;

    arr->dtype = dtype;
    arr->nd = ndim;

    size_t i;
    arr->size = 1;
    for(i = arr->nd; i > 0; i--){
        arr->dimensions[i-1] = dims[i-1];
        arr->strides[i-1] = arr->size*dtype->alignment;
        arr->size *= arr->dimensions[i-1];
    }

    arr->data = LNMem_alloc(arr->dtype->alignment * arr->size);
    if(!arr->data)
        return NULL;

    return arr;
}

Ndarray *LNArray_Zeros(const size_t *dims, size_t ndim, const LNTypeDescr *dtype){
    Ndarray *arr = LNArray_Alloc();
    if(!arr)
        return NULL;

    arr->dimensions = LNMem_alloc(sizeof(size_t)*ndim);
    if(!arr->dimensions)return NULL;
    
    arr->strides = LNMem_alloc(sizeof(long long)*ndim);
    if(!arr->strides)
        return NULL;

    arr->dtype = dtype;
    arr->nd = ndim;

    size_t i;
    arr->size = 1;
    for(i = arr->nd; i > 0; i--){
        arr->dimensions[i-1] = dims[i-1];
        arr->strides[i-1] = arr->size*dtype->alignment;
        arr->size *= arr->dimensions[i-1];
    }

    arr->data = LNMem_calloc(arr->dtype->alignment, arr->size);
    if(!arr->data)
        return NULL;

    return arr;
}

Ndarray *LNArray_Ones_(Ndarray *out, const size_t *dims, size_t ndim, const LNTypeDescr *dtype){
    out->dimensions = LNMem_alloc(sizeof(size_t)*ndim);
    if(!out->dimensions)return NULL;
    
    out->strides = LNMem_alloc(sizeof(long long)*ndim);
    if(!out->strides)
        return NULL;

    out->dtype = dtype;
    out->nd = ndim;

    size_t i;
    out->size = 1;
    for(i = out->nd; i > 0; i--){
        out->dimensions[i-1] = dims[i-1];
        out->strides[i-1] = out->size*dtype->alignment;
        out->size *= out->dimensions[i-1];
    }

    out->data = LNMem_alloc(out->dtype->alignment*out->size);
    if(!out->data)
        return NULL;
    
    switch(LNArray_TYPE(out)->id){
        case LN_BOOL:
        case LN_CHAR:
        case LN_UINT8:
        case LN_INT8:
            LNInt8Vector_Fill((int8_t*)out->data, 1, out->size);
            break;
        
        case LN_UINT16:
        case LN_INT16:
            LNInt16Vector_Fill((int16_t*)out->data, 1, out->size);
            break;
        
        case LN_UINT32:
        case LN_INT32:
            LNInt32Vector_Fill((int32_t*)out->data, 1, out->size);
            break;
        
        case LN_UINT64:
        case LN_INT64:
            LNInt64Vector_Fill((int64_t*)out->data, 1, out->size);
            break;
        
        case LN_FLOAT32:
            LNFloat32Vector_Fill((float32_t*)out->data, 1.0f, out->size);
            break;

        case LN_FLOAT64:
            LNFloat64Vector_Fill((float64_t*)out->data, 1.0f, out->size);
            break;
        
        case LN_COMPLEX64:
            LNComplex64Vector_Fill((complex64_t*)out->data, (complex64_t){1.0f,0.0f}, out->size);
            break;
        case LN_COMPLEX128:
            LNComplex128Vector_Fill((complex128_t*)out->data, (complex128_t){1.0f,0.0f}, out->size);
            break;
    }

    return out;
}

Ndarray *LNArray_Ones(const size_t *dims, size_t ndim, const LNTypeDescr *dtype){
    Ndarray *out = LNArray_Alloc();
    if(!out)
        return NULL;
    
    return LNArray_Ones_(out, dims, ndim, dtype);
}

Ndarray *LNArray_Range_(Ndarray *out, double start, double stop, double step, const LNTypeDescr *dtype){
    if(LNDType_IsInt(dtype->id) || LNDType_IsUInt(dtype->id)){
        start = (long long)start;
        stop = (long long)stop;
        step = (long long)step;
    }
    
    // Empty Ndarray.
    if(start > stop){
        out->dimensions = NULL;
        out->strides = NULL;
        out->data = NULL;
        out->size = 0;
        out->nd = 0;
        out->dtype = dtype;
        return out;
    }
    
    out->dimensions = LNMem_alloc(sizeof(size_t));
    if(!out->dimensions)
        return NULL;
    
    out->strides = LNMem_alloc(sizeof(long long));
    if(!out->strides)
        return NULL;
    
    out->size = (size_t)ceil((stop-start)/step);
    
    out->data = LNMem_alloc(dtype->alignment*out->size);
    if(!out->data)
        return NULL;
    
    out->nd = 1;
    out->dimensions[0] = out->size;
    out->strides[0] = dtype->alignment;
    out->dtype = dtype;

    size_t p;
    switch(dtype->id){
        case LN_BOOL:
        case LN_CHAR:
        case LN_UINT8:
        case LN_INT8:
            LNInt8Vector_Range((int8_t*)out->data, start, stop, step);
            break;
        
        case LN_UINT16:
        case LN_INT16:
            LNInt16Vector_Range((int16_t*)out->data, start, stop, step);
            break;
        
        case LN_UINT32:
        case LN_INT32:
            LNInt32Vector_Range((int32_t*)out->data, start, stop, step);
            break;
        
        case LN_UINT64:
        case LN_INT64:
            LNInt64Vector_Range((int64_t*)out->data, start, stop, step);
            break;
        
        case LN_FLOAT32:
            LNFloat32Vector_Range((float32_t*)out->data, start, stop, step);
            break;
        
        case LN_FLOAT64:
            LNFloat64Vector_Range((float64_t*)out->data, start, stop, step);
            break;
        
        case LN_COMPLEX64:
            LNComplex64Vector_Range((complex64_t*)out->data, start, stop, step);
            break;

        case LN_COMPLEX128:
            LNComplex128Vector_Range((complex128_t*)out->data, start, stop, step);
            break;
    }

    return out;
}

Ndarray *LNArray_Range(double start, double stop, double step, const LNTypeDescr *dtype){
    Ndarray *arr = LNArray_Alloc();
    if(!arr)
        return NULL;
    return LNArray_Range_(arr, start, stop, step, dtype);
}

#undef CASE
