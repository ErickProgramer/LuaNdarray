#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "arrayobj.h"
#include "arraydtype.h"
#include "error.h"
#include "arraycast.h"
#include "arrayaplly.h"

void LNArray_CastTo_(Ndarray *out, const Ndarray *arr, const LNTypeDescr *newtype, LNCast_t casttype){
    if(!LNArray_CanCast(arr->dtype, newtype, casttype)){
        char rule_repr[7];
        switch (casttype){
            case LNCAST_SAFE:
                strncpy(rule_repr, "safe", 7);
                break;
            case LNCAST_UNSAFE:
                strncpy(rule_repr, "unsafe", 7);
                break;
        }

        LNError_setFString("cannot cast array data from dtype(\"%s\") to dtype(\"%s\") according to the rule %s",
                            arr->dtype->name, newtype->name, rule_repr);
        return;
    }
    out->nd = arr->nd;
    out->dtype = newtype;

    out->strides = LNMem_alloc(sizeof(long long)*out->nd);
    if(!out->strides)
        return;
    
    out->dimensions = LNMem_alloc(sizeof(size_t)*out->nd);
    if(!out->dimensions)
        return;
    
    
    out->size = 1;
    size_t i;
    for(i = arr->nd; i > 0; i--){
        out->strides[i-1] = out->size * LNArray_ALIGNMENT(out);
        out->dimensions[i-1] = arr->dimensions[i-1];
        out->size *= out->dimensions[i-1];
    }

    out->data = LNMem_alloc(LNArray_ALIGNMENT(out)*out->size);
    if(!out->data)
        return;

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

int LNArray_CanCast(const LNTypeDescr *from, const LNTypeDescr *to, LNCast_t casttype){
    switch (casttype){
        case LNCAST_UNSAFE:
            return true;
        
        case LNCAST_SAFE:
            if(to->itemsize < from->itemsize)
                return false;
            if(LNDType_IsFloat(from->id) && (LNDType_IsInt(to->id) || LNDType_IsUInt(to->id)))
                return false;
            if(LNDType_IsInt(from->id) && LNDType_IsUInt(to->id))
                return false;
            if((LNDType_IsUInt(from->id) && LNDType_IsInt(to->id)) && to->itemsize < from->itemsize)
                return false;
            if((LNDType_IsComplex(from->id) && to->itemsize < from->itemsize))
                return false;
            return true;

        default:
            return -1;
    }
}