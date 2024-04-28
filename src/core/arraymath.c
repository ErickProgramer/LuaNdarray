#include<stdio.h>
#include<stdlib.h>

#include"arrayobj.h"
#include"arraymath.h"
#include"arrayindex.h"
#include"arrayaplly.h"
#include"arraycast.h"
#include"error.h"
#include"vector.h"
#include"buffer.h"

#define LNMath_(name) LN_CONCAT4(LN, Real, Array_, name)
#define LNRealDType LN_CONCAT2(LN, Real)

#include "generic/arraymath.c"
#include "LNGenerateAllTypes.h"

#undef LNMath_
#undef LNRealDType

void LNArray_BroadcastTo(Ndarray *out, Ndarray *arr, size_t *to, size_t nd, char *err_msg){
    // iterators
    size_t i, j;

    if(err_msg==NULL){
        LNBuffer *b = LNBuff_alloc();
        LNBuff_Init(b);

        LNBuff_addstring(b, "it was not possible to broadcast a shape array: ");

        LNBuff_addchar(b, '{');
        for(i=0; i<arr->nd; i++){
            LNBuff_addfstring(b, "%zu", arr->dimensions[i]);
            if(i < arr->nd-1){
                LNBuff_addchar(b, ',');
            }
        }
        LNBuff_addchar(b, '}');

        LNBuff_addstring(b, " to ");

        LNBuff_addchar(b, '{');
        for(i=0; i<nd; i++){
            LNBuff_addfstring(b, "%zu", to[i]);
            if(i < arr->nd-1){
                LNBuff_addchar(b, ',');
            }
        }
        LNBuff_addchar(b, '}');

        // error on build the error message
        if(LNError_Ocurred())
            return;
    
        err_msg = LNBuff_ADDR(b);
    }

    size_t new_ndim = LN_MAX(arr->nd, nd);

    size_t *dims1_expanded = LNMem_alloc(sizeof(size_t)*new_ndim);
    if(!dims1_expanded)
        return;

    size_t *dims2_expanded = LNMem_alloc(sizeof(size_t)*new_ndim);
    if(!dims2_expanded)
        return;

    size_t diff;

    if(arr->nd > nd){
        diff=arr->nd-nd;

        if(arr->dimensions)
            memcpy(dims1_expanded, arr->dimensions, sizeof(size_t)*arr->nd);
        else{
            for(i=0;i<new_ndim;i++){
                dims1_expanded[i]=1;
            }
        }
        for(i=0; i < diff; i++){
            dims2_expanded[i]=1;
        }
        memcpy(dims2_expanded+diff, to, sizeof(size_t)*nd);
    } else{
        diff=nd-arr->nd;

        memcpy(dims2_expanded, to, sizeof(size_t)*nd);
        for(i=0; i < diff; i++){
            dims1_expanded[i]=1;
        }
        if(arr->dimensions)
            memcpy(dims1_expanded+diff, arr->dimensions, sizeof(size_t)*new_ndim);
        else{
            for(i=diff;i<new_ndim;i++)
                dims1_expanded[i]=1;
        }
    }

    size_t *new_dims = LNMem_alloc(sizeof(size_t)*new_ndim);
    if(!new_dims)
        return;
    
    long long *new_strides = LNMem_alloc(sizeof(long long)*new_ndim);
    if(!new_strides)
        return;

    size_t new_size=1;
    for(i=new_ndim;i>0;i--){
        if(!((dims1_expanded[i-1] == dims2_expanded[i-1]) || (dims1_expanded[i-1]==1 || dims2_expanded[i-1]==1))){
            free(new_dims);
            free(dims1_expanded);
            free(dims2_expanded);
            LNError_setString(err_msg);
            return;
        }
        new_dims[i-1]=LN_MAX(dims1_expanded[i-1],dims2_expanded[i-1]);
        new_strides[i-1] = new_size*LNArray_ALIGNMENT(arr);
        new_size *= new_dims[i-1];
    }

    if(new_size==0){
        out->data = NULL;
        out->dimensions = dims1_expanded;
        out->nd = new_ndim;
        out->size = 0;
    
        out->strides = new_strides;
        out->dtype = arr->dtype;
        return;
    }

    out->data = LNMem_alloc(LNArray_ALIGNMENT(arr) * new_size);
    if(!out->data)
        return;

    out->size = arr->size;
    out->nd = new_ndim;

    out->dimensions = LNMem_alloc(sizeof(size_t)*new_ndim);
    if(!out->dimensions)
        return;
    out->strides = LNMem_alloc(sizeof(long)*new_ndim);
    if(!out->strides)
        return;
    memcpy(out->dimensions, dims1_expanded, sizeof(size_t)*new_ndim);

    out->dtype = arr->dtype;

    out->size = 1;
    for(i=new_ndim; i>0; i--){
        out->strides[i-1] = (long)(out->size*LNArray_ALIGNMENT(out));
        out->size *= new_dims[i-1];
    }
    out->size=arr->size;

    if(1 || arr->size==1){
        memcpy(out->data, arr->data, LNArray_ALIGNMENT(arr) * arr->size);
    }

    char *expanded = LNMem_alloc(LNArray_ALIGNMENT(out) * new_size);

    if(!expanded)
        return;

    size_t start, stop, cursize, expanded_size;

    i=new_ndim;
    while(i>0){
        i--;

        cursize=1;
        for(j=i; j<new_ndim; j++)
            cursize*=out->dimensions[j];
        
        diff = new_dims[i] - out->dimensions[i] + 1;

        expanded_size=0;
        for(start=0; start < out->size; start+=cursize){
            stop = start + cursize;
            char *expand = out->data+start*LNArray_ALIGNMENT(out);
            for(j=0;j<diff;j++){
                memcpy(expanded+expanded_size*LNArray_ALIGNMENT(out),
                       expand,
                       (stop-start)*LNArray_ALIGNMENT(out));
                expanded_size+=stop-start;
            }
        }

        out->dimensions[i] = new_dims[i];
        out->size = cursize*out->dimensions[i];
        memcpy(out->data, expanded, out->size*LNArray_ALIGNMENT(out));
    }
    out->strides = new_strides;
    free(expanded);
}

#define CASE(id, real, Real, op)\
    case id:{\
            if(LNArray_IsContiguous(arr1_solved) && LNArray_IsContiguous(arr2_solved)){\
                LN##Real##Vector_Add((real*)res->data, (real*)arr1->data, (real*)arr2->data, res->size);\
            } else {\
                size_t pos=0;\
                LN_ARRAY_APPLY2_CONTIG(arr1,arr2,\
                ((real*)res->data)[pos] = *(real*)item1 op *(real*)item2;\
                pos++;\
                )\
            }\
\
            break;\
        }

Ndarray *LNArray_Add(Ndarray *arr1, Ndarray *arr2){
    Ndarray *res = LNArray_Alloc();
    if(!res)
        return NULL;

    Ndarray *arr1_solved;
    Ndarray *arr2_solved;

    const LNTypeDescr *new_type=LNDType_Promote(arr1->dtype, arr2->dtype);
    if(LNArray_TYPE(arr1)->id==LNArray_TYPE(arr2)->id){
        arr1_solved = arr1;
        arr2_solved = arr2;
    } else{
        arr1_solved = LNArray_CastTo(arr1, new_type, LNCAST_SAFE);
        arr2_solved = LNArray_CastTo(arr2, new_type, LNCAST_SAFE);
        if(LNError_Ocurred()){
            return NULL;
        }
    }

    res->size = arr1->size;
    res->nd = arr1->nd;
    res->dimensions=arr1->dimensions;
    res->strides=arr1->strides;
    res->dtype=arr1->dtype;

    res->data = LNMem_alloc(arr1->dtype->itemsize * res->size);
    if(!res->data)
        return NULL;

    switch (arr1->dtype->id){
        case LN_CHAR:
        case LN_BOOL:
        case LN_UINT8:
        CASE(LN_INT8, int8_t, Int8, +);
        
        case LN_UINT16:
        CASE(LN_INT16, int16_t, Int16, +);
    
        case LN_UINT32:
        CASE(LN_INT32, int32_t, Int32, +);
        
        case LN_UINT64:
        CASE(LN_INT64, int64_t, Int64, +);
        
        CASE(LN_FLOAT32, float32_t, Float32, +);
        
        CASE(LN_FLOAT64, float64_t, Float64, +);
        
        case LN_COMPLEX64:
            if(LNArray_IsContiguous(arr1) && LNArray_IsContiguous(arr2)){
                LNComplex64Vector_Add((complex64_t*)res->data, (complex64_t*)arr1->data, (complex64_t*)arr2->data, res->size);
            } else{
                float32_t r1, i1, r2, i2;
                size_t pos=0;
                LN_ARRAY_APPLY2_CONTIG(arr1,arr2,
                    r1=((complex64_t*)item1)->realp;
                    i1=((complex64_t*)item1)->imagp;

                    r2=((complex64_t*)item2)->realp;
                    i2=((complex64_t*)item2)->imagp;
                    ((complex64_t*)res->data)[pos].realp = r1+r2;
                    ((complex64_t*)res->data)[pos].imagp = i1+i2;
                    pos++;
                )
            }
            break;

        case LN_COMPLEX128:
            if(LNArray_IsContiguous(arr1) && LNArray_IsContiguous(arr2)){
                LNComplex128Vector_Add((complex128_t*)res->data, (complex128_t*)arr1->data, (complex128_t*)arr2->data, res->size);
            } else{
                float64_t r1, i1, r2, i2;
                size_t pos=0;
                LN_ARRAY_APPLY2_CONTIG(arr1,arr2,
                    r1=((complex128_t*)item1)->realp;
                    i1=((complex128_t*)item1)->imagp;

                    r2=((complex128_t*)item2)->realp;
                    i2=((complex128_t*)item2)->imagp;
                    ((complex128_t*)res->data)[pos].realp = r1+r2;
                    ((complex128_t*)res->data)[pos].imagp = i1+i2;
                    pos++;
                )
            }
            break;
        
    }

    return res;
}


Ndarray *LNArray_MaxAxis(Ndarray *arr, long long axis){
    return arr->dtype->max_axis_func(arr, axis);
}

Ndarray *LNArray_Max(Ndarray *arr){
    return arr->dtype->max_func(arr);
}

Ndarray *LNArray_MinAxis(Ndarray *arr, long long axis){
    return arr->dtype->max_axis_func(arr, axis);
}

Ndarray *LNArray_Min(Ndarray *arr){
    return arr->dtype->min_func(arr);
}

Ndarray *LNArray_SumAxis(Ndarray *arr, long long axis){
    return arr->dtype->sum_axis_func(arr, axis);
}

Ndarray *LNArray_Sum(Ndarray *arr){
    return arr->dtype->sum_func(arr);
}
