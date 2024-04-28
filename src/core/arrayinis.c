#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

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

    arr->data = LNMem_alloc(arr->dtype->alignment*arr->size);
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

Ndarray *LNArray_Ones(const size_t *dims, size_t ndim, const LNTypeDescr *dtype){
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

    arr->data = LNMem_alloc(arr->dtype->alignment*arr->size);
    if(!arr->data)
        return NULL;
    
    switch(LNArray_TYPE(arr)->id){
        case LN_BOOL:
        case LN_CHAR:
        case LN_UINT8:
        case LN_INT8:
            LNInt8Vector_Fill((int8_t*)arr->data, 1, arr->size);
            break;
        
        case LN_UINT16:
        case LN_INT16:
            LNInt16Vector_Fill((int16_t*)arr->data, 1, arr->size);
            break;
        
        case LN_UINT32:
        case LN_INT32:
            LNInt32Vector_Fill((int32_t*)arr->data, 1, arr->size);
            break;
        
        case LN_UINT64:
        case LN_INT64:
            LNInt64Vector_Fill((int64_t*)arr->data, 1, arr->size);
            break;
        
        case LN_FLOAT32:
            LNFloat32Vector_Fill((float32_t*)arr->data, 1.0f, arr->size);
            break;
        case LN_FLOAT64:
            LNFloat64Vector_Fill((float64_t*)arr->data, 1.0f, arr->size);
            break;
        
        case LN_COMPLEX64:
            LNComplex64Vector_Fill((complex64_t*)arr->data, (complex64_t){1.0f,0.0f}, arr->size);
            break;
        case LN_COMPLEX128:
            LNComplex128Vector_Fill((complex128_t*)arr->data, (complex128_t){1.0f,0.0f}, arr->size);
            break;
    }

    return arr;
}

// #define CASE(id, type, cast)\
//     case id:\
//         for(i=start, p=0; ((cast)i) < ((cast)(stop)); i+=((cast)(step)), p++){\
//             printf("%lld\n",((cast)i));\
//             ((type*)arr->data)[p] = (type)(cast)i;\
//         }\
//         break\

Ndarray *LNArray_Range(double start, double stop, double step, const LNTypeDescr *dtype){
    Ndarray *arr = LNArray_Alloc();
    if(!arr)
        return NULL;

    if(LNDType_IsInt(dtype->id) || LNDType_IsUInt(dtype->id)){
        start = (long long)start;
        stop = (long long)stop;
        step = (long long)step;
    }
    
    // Empty Ndarray.
    if(start > stop){
        arr->dimensions = NULL;
        arr->strides = NULL;
        arr->data = NULL;
        arr->size = 0;
        arr->nd = 0;
        arr->dtype = dtype;
        return arr;
    }
    
    arr->dimensions = LNMem_alloc(sizeof(size_t));
    if(!arr->dimensions)
        return NULL;
    
    arr->strides = LNMem_alloc(sizeof(long long));
    if(!arr->strides)
        return NULL;
    
    arr->size = (size_t)ceil((stop-start)/step);
    printf("%zu\n",arr->size);
    
    arr->data = LNMem_alloc(dtype->alignment*arr->size);
    if(!arr->data)
        return NULL;
    
    arr->nd = 1;
    arr->dimensions[0] = arr->size;
    arr->strides[0] = dtype->alignment;
    arr->dtype = dtype;

    // LNArray_Debug(arr);

    size_t p;
    switch(dtype->id){
        case LN_BOOL:
        case LN_CHAR:
        case LN_UINT8:
        case LN_INT8:
            LNInt8Vector_Range((int8_t*)arr->data, start, stop, step);
            break;
        
        case LN_UINT16:
        case LN_INT16:
            LNInt16Vector_Range((int16_t*)arr->data, start, stop, step);
            break;
        
        case LN_UINT32:
        case LN_INT32:
            LNInt32Vector_Range((int32_t*)arr->data, start, stop, step);
            break;
        
        case LN_UINT64:
        case LN_INT64:
            LNInt64Vector_Range((int64_t*)arr->data, start, stop, step);
            break;
        
        case LN_FLOAT32:
            LNFloat32Vector_Range((float32_t*)arr->data, start, stop, step);
            break;
        
        case LN_FLOAT64:
            LNFloat64Vector_Range((float64_t*)arr->data, start, stop, step);
            break;
        
        case LN_COMPLEX64:
            LNComplex64Vector_Range((complex64_t*)arr->data, start, stop, step);
            break;
        
        case LN_COMPLEX128:
            LNComplex128Vector_Range((complex128_t*)arr->data, start, stop, step);
            break;

        // case LN_INT8:{
        //     long long _start, _stop, _step;
        //     _start=(long long)start;
        //     _stop=(long long)stop;
        //     _step=(long long)step;
        //     long long i;
        //     for(i=_start,p=0; i < _stop; i+=_step,p++){
        //         printf("%d\n",(int8_t)i);
        //         ((int8_t*)arr->data)[p] = (int8_t)i;
        //     }
        // }
        // // CASE(LN_INT8, int8_t, long long);

        // // case LN_UINT16:
        // // CASE(LN_INT16, int16_t, long long);

        // // case LN_UINT32:
        // // CASE(LN_INT32, int32_t, long long);

        // // case LN_UINT64:
        // // CASE(LN_INT64, int64_t, long long);

        // // CASE(LN_FLOAT32, float32_t, double);
        // // CASE(LN_FLOAT64, float64_t, double);

        // case LN_COMPLEX64:{
        //     double i;
        //     for(i=start,p=0; i<stop; i+=step, p++){
        //         ((complex64_t*)arr->data)[p].realp = (float32_t)i;
        //         ((complex64_t*)arr->data)[p].imagp = 0.0f;
        //     }
        //     break;
        // }
        // case LN_COMPLEX128:{
        //     double i;
        //     for(i=start,p=0; i<stop; i+=step, p++){
        //         ((complex128_t*)arr->data)[p].realp = (float64_t)i;
        //         ((complex128_t*)arr->data)[p].imagp = 0.0f;
        //     }
        //     break;
        // }
    }

    return arr;
}

#undef CASE
