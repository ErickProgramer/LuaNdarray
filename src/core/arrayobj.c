#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <lua.h>
#include <lauxlib.h>

#include "arraytypes.h"
#include "error.h"
#include "arrayobj.h"
#include "vector.h"
#include "arrayindex.h"
#include "buffer.h"

void LNArray_Init(Ndarray *arr){
    arr->data=NULL;
    arr->dimensions=NULL;
    arr->strides=NULL;
    arr->dtype=NULL;

    arr->nd=0;
    arr->size=0;
}

void LNArray_Debug(const Ndarray *arr){
    size_t i;

    printf("*********************\n");
    printf("dimensions={");
    for(i=0;i<arr->nd;i++){
        printf("%zu\t",arr->dimensions[i]);
    }
    printf("}\n");
    printf("strides={");
    for(i=0;i<arr->nd;i++){
        printf("%lld\t",arr->strides[i]);
    }
    printf("}\n");
    printf("size=%zu\n",arr->size);
    printf("dtype=%s\n",LNArray_TYPE(arr)->name);

    LNBuffer repr;
    LNBuff_Init(&repr);
    for(i=0;i<arr->size;i++){
        LNBuff_addtype(&repr,arr->data+(i*arr->dtype->alignment),arr->dtype);
        LNBuff_addchar(&repr,',');
    }
    printf("arr->data={%s}\n",LNBuff_ADDR(&repr));

    printf("*********************\n");
}

Ndarray *LNArray_New(void *data, size_t *dims, long long *strides, size_t nd, const LNTypeDescr *dtype){
    Ndarray *arr = LNArray_Alloc();
    if(!arr)
        return NULL;

    arr->strides = strides;

    arr->dtype = dtype;

    arr->data = data;

    arr->dimensions = dims;
    arr->nd = nd;

    size_t i;
    arr->size = 1;
    if(!strides){
        arr->strides=LNMem_alloc(sizeof(long long)*nd);
        if(!arr->strides)
            return NULL;
        for(i = arr->nd; i > 0; i--){
            arr->strides[i-1] = arr->size*dtype->alignment;
            arr->size *= arr->dimensions[i-1];
        }
    }else{
        arr->strides = strides;
    }

    return arr;
}



Ndarray *LNArray_Copy(const Ndarray *arr){
    Ndarray *copy = LNArray_Alloc();
    if(!copy){
        return NULL;
    }

    copy->dtype = arr->dtype;
    copy->nd = arr->nd;
    copy->dimensions = arr->dimensions;
    copy->size = arr->size;

    copy->strides = LNMem_alloc(sizeof(size_t)*copy->nd);
    if(!copy->strides)
        return NULL;

    copy->data = LNMem_alloc(copy->dtype->alignment * copy->size);
    if(!copy->data)
        return NULL;

    if(LNArray_IsContiguous(arr) || arr->size==1){
        memcpy(copy->data, arr->data, arr->dtype->alignment * arr->size);
    } else{
        long i;
        for(i=0;i<arr->size;i+=arr->strides[arr->nd-1]){
            memcpy(copy->data+i, arr->data+i, LNArray_ALIGNMENT(arr));
        }
    }

    return copy;
}

Ndarray *LNArray_NewScalar(const void *data, const LNTypeDescr *dtype){
    Ndarray *arr = LNArray_Alloc();
    if(!arr)
        return NULL;
    LNArray_Init(arr);

    arr->data = LNMem_alloc(dtype->alignment);
    if(!arr->data)
        return NULL;

    arr->size=1;
    arr->dtype=dtype;

    memcpy(arr->data, data, arr->dtype->alignment);
    
    return arr;
}

Ndarray *LNArray_New1D(void *data, size_t ax0, const LNTypeDescr *dtype){
    Ndarray *arr = LNArray_Alloc();
    if(!arr)
        return NULL;

    arr->dtype = dtype;
    arr->size = ax0;
    arr->nd = 1;

    arr->dimensions = LNMem_alloc(sizeof(size_t));
    if(!arr->dimensions)
        return NULL;

    arr->strides = LNMem_alloc(sizeof(long long));
    if(!arr->strides)
        return NULL;

    *arr->dimensions = ax0;
    *arr->strides = LNArray_ALIGNMENT(arr);

    arr->data = data;

    return arr;
}

Ndarray *LNArray_New2D(void *data, size_t ax0, size_t ax1, const LNTypeDescr *dtype){
    Ndarray *arr = LNArray_Alloc();
    if(!arr)
        return NULL;

    arr->dimensions = LNMem_alloc(sizeof(size_t)*2);
    if(!arr->dimensions)
        return NULL;

    arr->strides = LNMem_alloc(sizeof(long long)*2);
    if(!arr->strides)    
        return NULL;

    arr->dimensions[0] = ax0;
    arr->dimensions[1] = ax1;

    arr->strides[1] = dtype->alignment;
    arr->strides[0] = arr->dimensions[0]*dtype->alignment;

    arr->dtype = dtype;
    arr->size = ax0*ax1;
    arr->nd = 2;

    arr->data = data;

    return arr;
}

int LNArray_IsContiguous(const Ndarray *arr){
    size_t i;
    size_t size=1;
    long long curstride;
    for(i=arr->nd; i>0; i--){
        curstride=size*LNArray_ALIGNMENT(arr);
        if(arr->strides[i-1] != curstride)
            return false;
        size*=arr->dimensions[i-1];
    }
    return true;
}

void LNArray_Free(Ndarray *arr){
    if(arr){
        if(arr->data)
            free(arr->data);
        if(arr->dimensions)
            free(arr->dimensions);
        if(arr->strides)
            free(arr->strides);
        free(arr);
    }
}
