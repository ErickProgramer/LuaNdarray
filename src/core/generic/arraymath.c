#ifndef LN_GENERIC_FILE
#define LN_GENERIC_FILE "generic/arraymath.c"
#else

#include "../arrayobj.h"
#include "../arrayaplly.h"
#include "../utils.h"

Ndarray *LNMath_(SumAxis)(Ndarray *arr, long long axis){
    Ndarray *res = LNArray_Alloc();
    if(!res)
        return NULL;

    res->nd = arr->nd - 1;
    res->size = arr->size / arr->dimensions[axis];

    res->dimensions = LNMem_alloc(sizeof(size_t)*res->nd);
    if(!res->dimensions)
        return NULL;

    res->strides = LNMem_alloc(sizeof(long long)*res->nd);
    if(!res->strides)
        return NULL;
    
    res->data = LNMem_alloc(sizeof(real) * res->size);
    if(!res->data)
        return NULL;
    
    res->dtype = LNRealDType;

    memcpy(res->dimensions, arr->dimensions, sizeof(size_t) * axis);
    memcpy(res->dimensions+axis, arr->dimensions+axis, sizeof(size_t) * (arr->nd-axis));

    long long i, j;
    size_t axis_size=1;
    for(i=axis; i < arr->nd; i++){
        axis_size *= arr->dimensions[i];
    }

    size_t curpos=0;
    for(i=0; i < arr->dimensions[axis]; i++){
        real *axis_data = (real*)(arr->data + (i * arr->strides[axis]));

        LN_VECTOR_APPLY_CONTIG(axis_data, arr->dimensions+axis, arr->strides+axis, res->nd,
            #if defined(LN_REAL_IS_COMPLEX)
            ((real*)res->data)[curpos].realp += (*(real*)item).realp;
            ((real*)res->data)[curpos].imagp += (*(real*)item).imagp;
            #else
            ((real*)res->data)[curpos] += *(real*)item;
            #endif
        );

        curpos++;
    }

    return res;
}

Ndarray *LNMath_(MaxAxis)(Ndarray *arr, long long axis){
    Ndarray *res = LNArray_Alloc();
    if(!res)
        return NULL;

    res->nd = arr->nd - 1;
    res->size = arr->size / arr->dimensions[axis];

    res->dimensions = LNMem_alloc(sizeof(size_t)*res->nd);
    if(!res->dimensions)
        return NULL;

    res->strides = LNMem_alloc(sizeof(long long)*res->nd);
    if(!res->strides)
        return NULL;
    
    res->data = LNMem_alloc(sizeof(real) * res->size);
    if(!res->data)
        return NULL;
    
    res->dtype = LNRealDType;

    memcpy(res->dimensions, arr->dimensions, sizeof(size_t) * axis);
    memcpy(res->dimensions+axis, arr->dimensions+axis, sizeof(size_t) * (arr->nd-axis));

    long long i, j;
    size_t axis_size=1;
    for(i=axis; i < arr->nd; i++){
        axis_size *= arr->dimensions[i];
    }

    size_t curpos=0;
    for(i=0; i < arr->dimensions[axis]; i++){
        real *axis_data = (real*)(arr->data + (i * arr->strides[axis]));
        real max_v = *axis_data;

        LN_VECTOR_APPLY_CONTIG(axis_data, arr->dimensions+axis, arr->strides+axis, res->nd,
            #if defined(LN_REAL_IS_COMPLEX)
            if(((*(real*)item).realp > max_v.realp) && ((*(real*)item).imagp > max_v.imagp)){
                max_v = *(real*)item;
            }
            #else
            if(*(real*)item > max_v){
                max_v = *(real*)item;
            }
            #endif
        );

        curpos++;
    }

    return res;
}

Ndarray *LNMath_(MinAxis)(Ndarray *arr, long long axis){
    Ndarray *res = LNArray_Alloc();
    if(!res)
        return NULL;

    res->nd = arr->nd - 1;
    res->size = arr->size / arr->dimensions[axis];

    res->dimensions = LNMem_alloc(sizeof(size_t)*res->nd);
    if(!res->dimensions)
        return NULL;

    res->strides = LNMem_alloc(sizeof(long long)*res->nd);
    if(!res->strides)
        return NULL;
    
    res->data = LNMem_alloc(sizeof(real) * res->size);
    if(!res->data)
        return NULL;
    
    res->dtype = LNRealDType;

    memcpy(res->dimensions, arr->dimensions, sizeof(size_t) * axis);
    memcpy(res->dimensions+axis, arr->dimensions+axis, sizeof(size_t) * (arr->nd-axis));

    long long i, j;
    size_t axis_size=1;
    for(i=axis; i < arr->nd; i++){
        axis_size *= arr->dimensions[i];
    }

    // LN_ARRAY_APPLY_D(arr, axis,
        
    // )
    // size_t curpos=0;
    // for(i=0; i < arr->dimensions[axis]; i++){
    //     real *axis_data = (real*)(arr->data + (i * arr->strides[axis]));
    //     real min_v = *axis_data;

    //     LN_VECTOR_APPLY_D(axis_data)
    //     // LN_VECTOR_APPLY_CONTIG(axis_data, arr->dimensions+axis, arr->strides+axis, res->nd,
    //     //     #if defined(LN_REAL_IS_COMPLEX)
    //     //     if(((*(real*)item).realp < min_v.realp) && ((*(real*)item).imagp < min_v.imagp)){
    //     //         min_v = *(real*)item;
    //     //     }
    //     //     #else
    //     //     if(*(real*)item < min_v){
    //     //         min_v = *(real*)item;
    //     //     }
    //     //     #endif
    //     // )

    //     curpos++;
    // }

    return res;
}

Ndarray *LNMath_(Max)(Ndarray *arr){
    Ndarray *res = LNArray_Alloc();
    if(!res)
        return NULL;

    res->data = LNMem_alloc(sizeof(real));
    if(!res->data)
        return NULL;

    *(real*)res->data = *(real*)arr->data;

    res->nd = 0;
    res->size = 1;
    res->strides = NULL;
    res->dimensions = NULL;
    res->dtype = LNRealDType;

    LN_ARRAY_APPLY_CONTIG(arr,
        #if defined(LN_REAL_IS_COMPLEX)
        if((*(real*)item).realp > (*(real*)res->data).realp && (*(real*)item).imagp > (*(real*)res->data).imagp){
            *(real*)res->data = *(real*)item;
        }
        #else
        if(*(real*)item > *(real*)res->data){
            *(real*)res->data = *(real*)item;
        }
        #endif
    );

    return res;
}

Ndarray *LNMath_(Min)(Ndarray *arr){
    Ndarray *res = LNArray_Alloc();
    if(!res)
        return NULL;

    res->data = LNMem_alloc(sizeof(real));
    if(!res->data)
        return NULL;

    *(real*)res->data = *(real*)arr->data;

    res->nd = 0;
    res->size = 1;
    res->strides = NULL;
    res->dimensions = NULL;
    res->dtype = LNRealDType;

    real max_v = *(real*)arr->data;
    LN_ARRAY_APPLY_CONTIG(arr,
        #if defined(LN_REAL_IS_COMPLEX)
        if((*(real*)item).realp < (*(real*)res->data).realp && (*(real*)item).imagp < (*(real*)res->data).imagp){
            *(real*)res->data = *(real*)item;
        }
        #else
        if(*(real*)item < *(real*)res->data){
            *(real*)res->data = *(real*)item;
        }
        #endif
    );

    return res;
}

Ndarray *LNMath_(Sum)(Ndarray *arr){
    Ndarray *res = LNArray_Alloc();
    if(!res)
        return NULL;

    res->data = LNMem_alloc(sizeof(real));
    if(!res->data)
        return NULL;

    res->nd = 0;
    res->size = 1;
    res->strides = NULL;
    res->dimensions = NULL;
    res->dtype = LNRealDType;

    #if defined(LN_REAL_IS_COMPLEX)
    *(real*)res->data = (real){.realp=0, .imagp=0};
    #else
    *(real*)res->data = 0;
    #endif
    LN_ARRAY_APPLY_CONTIG(arr,
        #if defined(LN_REAL_IS_COMPLEX)
        (*(real*)res->data).realp += (*(real*)item).realp;
        (*(real*)res->data).imagp += (*(real*)item).imagp;
        #else
        *(real*)res->data += *(real*)item;
        #endif
    );

    return res;
}

#endif