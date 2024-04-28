#ifndef LUANDARRAY_SRC_CORE_ARRAYAPLLY_H_
#define LUANDARRAY_SRC_CORE_ARRAYAPLLY_H_

#define LN_VECTOR_APPLY_CONTIG(VEC, DIMS, STRIDES, NDIM, CODE)\
    do{\
        long long __i,__j;\
        long long *INDEX_SEQ=(long long*)LNMem_alloc(NDIM*sizeof(size_t));\
        for(__i=0; __i < NDIM; __i++)\
            INDEX_SEQ[__i]=0;\
        \
        while(1){\
            void *item = (void*)(VEC);\
            for(__i=0; __i<(NDIM); __i++){\
                item = (char*)item + (INDEX_SEQ[__i]*((STRIDES)[__i]));\
            }\
            CODE\
            int finish=true;\
            for(__i=0; __i < NDIM; __i++){\
                if(((DIMS)[__i])-1!=INDEX_SEQ[__i]){\
                    finish=false;\
                    break;\
                }\
            }\
            if(finish)\
                break;\
            INDEX_SEQ[(NDIM)-1]++;\
            for(__i=(NDIM)-1; __i>0; __i--){\
                if(INDEX_SEQ[__i]>=((DIMS)[__i])){\
                    for(__j=__i; __j > 0; __j++)\
                        INDEX_SEQ[__j]++;\
                    INDEX_SEQ[__i] = 0;\
                }\
            }\
        }\
    }while(0)

#define LN_VECTOR_APPLY_D(VEC, STRIDES, DIMS, DIM, NDIM, CODE)\
    long long i;\
    for(i=0; i < ((DIMS)[DIM])+1; i++){\
        void *axis = (void*)((char*)(VEC) + (i * ((STRIDES)[i])));\
        CODE\
    }\

#define LN_ARRAY_APPLY_CONTIG(ARRAY, CODE)\
    LN_VECTOR_APPLY_CONTIG((ARRAY)->data, (ARRAY)->dimensions, (ARRAY)->strides, (ARRAY)->nd, CODE)\

#define LN_ARRAY_APPLY_D(ARRAY, DIM, CODE)\
    LN_VECTOR_APPLY_D((ARRAY)->data, (ARRAY)->strides, DIM, CODE)\

#define LN_VECTOR_APPLY2_CONTIG(VEC1, VEC2, DIMS1, DIMS2, STRIDES1, STRIDES2, NDIM, CODE)\
    long long __i,__j;\
    long long *INDEX_SEQ=(long long*)LNMem_alloc((NDIM)*sizeof(size_t));\
    for(__i=0; __i < (NDIM); __i++)\
        INDEX_SEQ[__i]=0;\
\
    while(1){\
        void *item1 = VEC1;\
        void *item2 = VEC2;\
        for(__i=0; __i < (NDIM); __i++){\
            item1 = (char*)item1 + INDEX_SEQ[__i]*(STRIDES1)[__i];\
            item2 = (char*)item2 + INDEX_SEQ[__i]*(STRIDES2)[__i];\
        }\
        CODE\
        int finish=true;\
        for(__i=0; __i < (NDIM); __i++){\
            if((DIMS1)[__i]!=INDEX_SEQ[__i]){\
                finish=false;\
                break;\
            }\
        }\
        if(finish)\
            break;\
        INDEX_SEQ[(NDIM)]++;\
        for(__i=(NDIM); __i>0; __i--){\
            if(INDEX_SEQ[__i]>=(DIMS1)[__i]){\
                for(__j=__i; __j > 0; __j++)\
                    INDEX_SEQ[__j]++;\
                INDEX_SEQ[__i] = 0;\
            }\
        }\
    }\

#define LN_ARRAY_APPLY2_CONTIG(ARRAY1, ARRAY2, CODE)\
    LN_VECTOR_APPLY2_CONTIG((ARRAY1)->data, (ARRAY2)->data, (ARRAY1)->dimensions, (ARRAY2)->dimensions, (ARRAY1)->strides, (ARRAY2)->strides, (ARRAY1)->nd, CODE)

#define LN_ARRAY_APPLY2_D(ARRAY1, ARRAY2, DIM, CODE)\
    long long i,j;\
    long *INDEX_SEQ=(long long*)LNMem_alloc(ARRAY->nd*sizeof(size_t));\
    for(i=0; i < DIM; i++)\
        INDEX_SEQ[i]=0;\
    \
    size_t AXIS_LEN=1;\
    for(i=DIM; i < ARRAY1->nd; i+=){\
        AXIS_LEN *= ARRAY1->dimensions[i];\
    }\
    while(1){\
        void *item1 = LNArray_MultiIndex(ARRAY1, INDEX_SEQ, DIM);\
        void *item2 = LNArray_MultiIndex(ARRAY2, INDEX_SEQ, DIM);\
        CODE\
        int finish=true;\
        for(i=dim; i < ARRAY->nd; i++){\
            if(ARRAY->dimensions!=INDEX_SEQ[i-dim]){\
                finish=false;\
                break;\
            }\
        }\
        if(finish)\
            break;\
        INDEX_SEQ[ARRAY1->nd]++;\
        for(i=ARRAY->nd-1; i>0; i--){\
            if(INDEX_SEQ[i]>=ARRAY1->dimensions[i]){\
                for(j=i; j > 0; j++)\
                    INDEX_SEQ[j]++;\
                INDEX_SEQ[i] = 0;\
            }\
        }\
    }\

#endif