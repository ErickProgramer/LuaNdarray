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

#define TEST printf("TEST on %d\n", __LINE__)

void LNArray_BroadcastTo(Ndarray *out, Ndarray *arr, size_t *to, size_t nd){
    size_t i, j;


    size_t new_ndim = LN_MAX(arr->nd, nd);

    size_t *arrdims_expanded = LNMem_alloc(sizeof(size_t) * new_ndim);
    if(!arrdims_expanded)
        return;

    size_t *to_expanded = LNMem_alloc(sizeof(size_t) * new_ndim);
    if(!to_expanded)
        return;

    for(i = 0; i < new_ndim; i++){
        arrdims_expanded[i] = 1;
        to_expanded[i] = 1;
    }

    memcpy(arrdims_expanded + (new_ndim - arr->nd), arr->dimensions, sizeof(size_t)*arr->nd);
    memcpy(to_expanded + (new_ndim - nd), to, sizeof(size_t)*nd);

    for(i = new_ndim; i > 0; i++){
        if(!((arrdims_expanded[i-1] == to_expanded[i-1]) || (arrdims_expanded[i-1] == 1))){
            LNBuffer errmsg;
            LNBuff_Init(&errmsg);

            LNBuff_addchar(&errmsg, '{');
            for(j = 0; j < arr->nd; j++)
                LNBuff_addfstring(&errmsg, "%zu,", arr->dimensions[j]);
            LNBuff_addstring(&errmsg, "} ");

            LNBuff_addchar(&errmsg, '{');
            for(j = 0; j < nd; j++)
                LNBuff_addfstring(&errmsg, "%zu,", to[j]);
            LNBuff_addchar(&errmsg, '}');
            LNBuff_addfstring(&errmsg, " shapes at dim %zu does not match", i-1);

            LNError_setString(LNBuff_ADDR(&errmsg));
            return;
        }
    }

    out->dtype = arr->dtype;
    out->nd = new_ndim;
    out->dimensions = LNMem_alloc(sizeof(size_t) * out->nd);
    if(!out->dimensions)
        return;

    out->strides = LNMem_alloc(sizeof(long long) * out->nd);
    if(!out->strides)
        return;


    size_t new_size = 1;
    out->size = 1;
    for(i = out->nd; i > 0; i--){
        out->dimensions[i-1] = arrdims_expanded[i-1];
        out->strides[i-1] = out->size * LNArray_ALIGNMENT(out);
        out->size *= out->dimensions[i-1];
        new_size *= to_expanded[i-1];
    }

    out->data = LNMem_alloc(LNArray_ALIGNMENT(out) * new_size);
    if(!out->data)
        return;
    memcpy(out->data, arr->data, LNArray_ALIGNMENT(arr) * arr->size);

    size_t expanded_size;
    char *expanded = LNMem_alloc(LNArray_ALIGNMENT(out) * new_size);

    if(!LNArray_IsContiguous(arr)){

    }

    i = new_ndim;
    while(i > 0){
        i--;

        size_t diff = to_expanded[i] - arrdims_expanded[i] + 1;
        expanded_size = 0;
        LN_ARRAY_APPLY_D(out, i,
            for(j = 0; j < diff; j++){
                memcpy(expanded + expanded_size, item, LNArray_ALIGNMENT(out)*__axis_size);
                expanded_size += __axis_size;
            }
        );

        out->size /= out->dimensions[i];
        out->dimensions[i] = to_expanded[i];
        out->size *= out->dimensions[i];
    
        memcpy(out->data, expanded, LNArray_ALIGNMENT(out)*expanded_size);
    }

    free(to_expanded);
    free(arrdims_expanded);
    free(expanded);
//     printf("ok\n");
//     // iterators
//     size_t i, j;

//     size_t new_ndim = LN_MAX(arr->nd, nd);

//     size_t *dims1_expanded = LNMem_alloc(sizeof(size_t)*new_ndim);
//     if(!dims1_expanded)
//         return;

//     size_t *to_expanded = LNMem_alloc(sizeof(size_t)*new_ndim);
//     if(!dims2_expanded)
//         return;

//     size_t diff;

//     if(arr->nd > nd){
//         diff=arr->nd-nd;

//         if(arr->dimensions)
//             memcpy(dims1_expanded, arr->dimensions, sizeof(size_t)*arr->nd);
//         else{
//             for(i=0;i<new_ndim;i++){
//                 dims1_expanded[i]=1;
//             }
//         }
//         for(i=0; i < diff; i++){
//             dims2_expanded[i]=1;
//         }
//         memcpy(to_expanded+diff, to, sizeof(size_t)*nd);
//     } else{
//         diff=nd-arr->nd;

//         memcpy(to_expanded, to, sizeof(size_t)*nd);
//         for(i=0; i < diff; i++){
//             dims1_expanded[i]=1;
//         }
//         if(arr->dimensions)
//             memcpy(dims1_expanded+diff, arr->dimensions, sizeof(size_t)*new_ndim);
//         else{
//             for(i=diff;i<new_ndim;i++)
//                 dims1_expanded[i]=1;
//         }
//     }

//     long long *new_strides = (long long*)malloc(sizeof(long long)*new_ndim);

//     size_t new_size=1;
//     for(i=new_ndim;i>0;i--){
//         if(!((to_expanded[i-1] == dims1_expanded[i-1]) || dims1_expanded[i-1] == 1)){
//             TEST;
//             LNError_setFString("Error on axis %zu: size (%zu) must be the same as (%zu) or the old size must be 1", i-1, dims2_expanded[i-1], dims1_expanded[i-1]);
//             free(new_strides);
//             free(dims1_expanded);
//             free(dims2_expanded);
//             LNArray_Free(out);
//             return;
//         }

//         new_strides[i-1] = new_size*LNArray_ALIGNMENT(arr);
//         new_size *= dims2_expanded[i-1];
//     }
//     TEST;

//     out->dtype = arr->dtype;
//     out->nd = new_ndim;

//     out->dimensions = LNMem_alloc(sizeof(size_t) * new_ndim);
//     if(!out->dimensions)
//         return;
    
//     out->strides = LNMem_alloc(sizeof(long long) * new_ndim);
//     if(!out->strides)
//         return;

//     out->size = 1;
//     for(i = new_ndim; i > 0; i--){
//         out->strides[i-1] = out->size * LNArray_ALIGNMENT(arr);
//         out->dimensions[i-1] = dims1_expanded[i-1];
//         out->size *= dims2_expanded[i-1];
//     }

//     out->data = LNMem_alloc(LNArray_ALIGNMENT(arr)*new_size);
//     if(!out->data)
//         return;
//     memcpy(out->data, arr->data, LNArray_ALIGNMENT(arr)*arr->size);

//     char *expanded = LNMem_alloc(LNArray_ALIGNMENT(arr)*out->size);
//     if(!expanded)
//         return;
    
//     size_t expanded_size = 0;

//     i = new_ndim;
//     while(i > 0){
//         i--;

//         diff = dims2_expanded[i] - dims1_expanded[i] + 1;
//         LN_ARRAY_APPLY_D(out, i,
//             for(j = 0; j < diff; j++){
//                 memcpy(expanded + expanded_size, item, __axis_size * LNArray_ALIGNMENT(arr));
//                 expanded_size += __axis_size;
//             }
//         );
    
//         memcpy(out->data, expanded, out->size * LNArray_ALIGNMENT(arr));
//         out->dimensions[i] = dims2_expanded[i];

//         expanded_size = 0;
//     }
// }

// #define CASE(id, real, Real, op)\
//     case id:{\
//             if(LNArray_IsContiguous(arr1_solved) && LNArray_IsContiguous(arr2_solved)){\
//                 LN##Real##Vector_Add((real*)res->data, (real*)arr1->data, (real*)arr2->data, res->size);\
//             } else {\
//                 size_t pos=0;\
//                 LN_ARRAY_APPLY2_CONTIG(arr1,arr2,\
//                 ((real*)res->data)[pos] = *(real*)item1 op *(real*)item2;\
//                 pos++;\
//                 )\
//             }\
// \
//             break;\
//         }

// Ndarray *LNArray_Add(Ndarray *arr1, Ndarray *arr2){
//     Ndarray *res = LNArray_Alloc();
//     if(!res)
//         return NULL;

//     Ndarray *arr1_solved;
//     Ndarray *arr2_solved;

//     const LNTypeDescr *new_type=LNDType_Promote(arr1->dtype, arr2->dtype);
//     if(LNArray_TYPE(arr1)->id==LNArray_TYPE(arr2)->id){
//         arr1_solved = arr1;
//         arr2_solved = arr2;
//     } else{
//         arr1_solved = LNArray_CastTo(arr1, new_type, LNCAST_SAFE);
//         arr2_solved = LNArray_CastTo(arr2, new_type, LNCAST_SAFE);
//         if(LNError_Ocurred()){
//             return NULL;
//         }
//     }

//     res->size = arr1->size;
//     res->nd = arr1->nd;
//     res->dimensions=arr1->dimensions;
//     res->strides=arr1->strides;
//     res->dtype=arr1->dtype;

//     res->data = LNMem_alloc(arr1->dtype->itemsize * res->size);
//     if(!res->data)
//         return NULL;

//     switch (arr1->dtype->id){
//         case LN_CHAR:
//         case LN_BOOL:
//         case LN_UINT8:
//         CASE(LN_INT8, int8_t, Int8, +);
        
//         case LN_UINT16:
//         CASE(LN_INT16, int16_t, Int16, +);
    
//         case LN_UINT32:
//         CASE(LN_INT32, int32_t, Int32, +);
        
//         case LN_UINT64:
//         CASE(LN_INT64, int64_t, Int64, +);
        
//         CASE(LN_FLOAT32, float32_t, Float32, +);
        
//         CASE(LN_FLOAT64, float64_t, Float64, +);
        
//         case LN_COMPLEX64:
//             if(LNArray_IsContiguous(arr1) && LNArray_IsContiguous(arr2)){
//                 LNComplex64Vector_Add((complex64_t*)res->data, (complex64_t*)arr1->data, (complex64_t*)arr2->data, res->size);
//             } else{
//                 float32_t r1, i1, r2, i2;
//                 size_t pos=0;
//                 LN_ARRAY_APPLY2_CONTIG(arr1,arr2,
//                     r1=((complex64_t*)item1)->realp;
//                     i1=((complex64_t*)item1)->imagp;

//                     r2=((complex64_t*)item2)->realp;
//                     i2=((complex64_t*)item2)->imagp;

//                     ((complex64_t*)res->data)[pos].realp = r1+r2;
//                     ((complex64_t*)res->data)[pos].imagp = i1+i2;
//                     pos++;
//                 )
//             }
//             break;

//         case LN_COMPLEX128:
//             if(LNArray_IsContiguous(arr1) && LNArray_IsContiguous(arr2)){
//                 LNComplex128Vector_Add((complex128_t*)res->data, (complex128_t*)arr1->data, (complex128_t*)arr2->data, res->size);
//             } else{
//                 float64_t r1, i1, r2, i2;
//                 size_t pos=0;
//                 LN_ARRAY_APPLY2_CONTIG(arr1,arr2,
//                     r1=((complex128_t*)item1)->realp;
//                     i1=((complex128_t*)item1)->imagp;

//                     r2=((complex128_t*)item2)->realp;
//                     i2=((complex128_t*)item2)->imagp;
//                     ((complex128_t*)res->data)[pos].realp = r1+r2;
//                     ((complex128_t*)res->data)[pos].imagp = i1+i2;
//                     pos++;
//                 )
//             }
//             break;
        
//     }

//     return res;
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
