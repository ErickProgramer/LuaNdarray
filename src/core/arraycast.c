#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "arrayobj.h"
#include "arraydtype.h"
#include "error.h"
#include "arraycast.h"
#include "arrayaplly.h"

void LNArray_CastTo_(Ndarray *out, const Ndarray *arr, const LNTypeDescr *newtype, LNCast_t casttype){
    if(casttype == LNCAST_SAFE && (!LNDType_CastIsSafe(LNArray_TYPE(arr), newtype))){
        LNError_setString("unsafe casting");
        return;
    }
    out->data = LNMem_alloc(newtype->alignment * arr->size);
    if(!out->data)
        return;

    out->nd = arr->nd;
    out->dtype = newtype;

    out->strides = LNMem_alloc(sizeof(long long)*out->nd);
    if(!out->strides)
        return;
    
    out->dimensions = LNMem_alloc(sizeof(size_t)*out->nd);
    if(!out->dimensions)
        return;
    
    out->size=arr->size;
    
    memcpy(out->dimensions, arr->dimensions, sizeof(size_t)*arr->nd);
    memcpy(out->strides, arr->strides, sizeof(long long)*arr->nd);

    out->data = LNMem_alloc(LNArray_ALIGNMENT(out)*out->size);
    if(!out->data)
        return;

    // LN_ARRAY_APPLY_CONTIG(arr,
    // )
    // arr->dtype->castfunc(out->data, arr->data, newtype->id, arr->size);
    arr->dtype->castfunc(out, arr, newtype->id);
}

Ndarray *LNArray_CastTo(const Ndarray *arr, const LNTypeDescr *newtype, LNCast_t casttype){
    Ndarray *out = LNArray_Alloc();
    if(!out)
        return NULL;

    LNArray_CastTo_(out, arr, newtype, casttype);
    if(LNError_Ocurred()){
        LNArray_Free(out);
        return NULL;
    }

    return out;
}