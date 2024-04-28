#include<stdio.h>
#include<stdlib.h>

#include "arrayindex.h"
#include "error.h"
#include "arrayobj.h"

Ndarray *LNArray_Index_(Ndarray *out, Ndarray *arr, long long idx){
    if(arr->nd == 0){
        LNError_setString("attemp to index a 0-dimensional array");
        return NULL;
    } else if(idx >= LNArray_LEN(arr) || -idx >= LNArray_LEN(arr)){
        LNError_setFString("position %lld out of bounds", idx);
        return NULL;
    }

    if(idx < 0){
        idx=arr->dimensions[0]+idx;
    }

    out->dtype = arr->dtype;
    out->size = arr->size / arr->dimensions[0];
    out->nd = arr->nd-1;
    out->dimensions = arr->dimensions + 1;
    out->strides = arr->strides+1;
    out->data = arr->data+(idx*arr->strides[0]);

    return out;
}

Ndarray *LNArray_Index(Ndarray *arr, long long idx){
    Ndarray *res = LNArray_Alloc();
    if(!res)
        return NULL;
    return LNArray_Index_(res, arr, idx);
}

Ndarray *LNArray_MultiIndex(Ndarray *arr, long long *idxs, size_t nidxs){
    size_t i;
    for(i = 0; i < nidxs; i++){
        arr = LNArray_Index(arr, idxs[i]);
        if(!arr)
            return NULL;
    }
    return arr;
}

void *LNArray_MultiIndexItem(Ndarray *arr, long long *idxs){
    size_t i;
    long long pos=idxs[0]*arr->strides[0];

    for(i = 1; i < arr->nd; i++)
        pos+=idxs[i]*arr->strides[i];

    return (void*)(arr->data + pos);
}
