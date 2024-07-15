#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>

#include "arrayindex.h"
#include "error.h"
#include "arrayobj.h"
#include "arrayaplly.h"

static int INDEX_BASE;

int LNGetIndexStart(){
    return INDEX_BASE;
}

void LNIndexStartZero(bool_t s){
    INDEX_BASE = s == 0;
}

Ndarray *LNArray_Index_(Ndarray *out, Ndarray *arr, long long idx){
    if(arr->nd == 0){
        LNError_setString("attemp to index a 0-dimensional array");
        return NULL;
    } else if(idx >= LNArray_LEN(arr) || -(idx + 1) >= LNArray_LEN(arr)){
        LNError_setFString("index %lld is out of bounds for axis 0 with size %zu", idx, arr->dimensions[0]);
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

void LNArray_MapDim(Ndarray *arr, long long dim){
    long long i;
    LN_ARRAY_APPLY_D(arr, dim,
        printf("{");
        for(i = 0; i < __axis_size; i++){
            printf("%d, ", ((char*)item)[i]);
        }
        printf("}\n");
    );
}
