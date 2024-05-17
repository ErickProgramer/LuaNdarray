#ifndef LUANDARRAY_SRC_CORE_ARRAYAPLLY_H_
#define LUANDARRAY_SRC_CORE_ARRAYAPLLY_H_

#define LN_VECTOR_APPLY_CONTIG(VEC, DIMS, STRIDES, NDIM, CODE)\
    do{\
        long long __i,__j;\
        long long *INDEX_SEQ=(long long*)LNMem_alloc((NDIM)*sizeof(long long));\
        for(__i=0; __i < (NDIM); __i++)\
            INDEX_SEQ[__i]=0;\
        \
        while(1){\
            void *item = (void*)(VEC);\
            for(__i=0; __i<(NDIM); __i++){\
                item = (void*)((char*)item + (INDEX_SEQ[__i]*((STRIDES)[__i])));\
            }\
            CODE\
            int finish=true;\
            for(__i=0; __i < (NDIM); __i++){\
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
                    for(__j=__i; __j >= 0; __j--){\
                        INDEX_SEQ[__j]++;\
                    }\
                    INDEX_SEQ[__i] = 0;\
                }\
            }\
        }\
    }while(0)


#define LN_VECTOR_APPLY_D(VEC, STRIDES, DIMS, DIM, NDIM, CODE)\
    do{\
        long __i, __j;\
        size_t __axis_size = 1;\
        size_t __axis_n = 1;\
        for(__i = 0; __i < (DIM)+1; __i++)\
            __axis_n *= (DIMS)[__i];\
        for(__i = (DIM)+1; __i < (NDIM); __i++)\
            __axis_size *= (DIMS)[__i];\
        void *item = ((void*)VEC);\
        for(__i = 0; __i < __axis_n; __i++){\
            CODE\
            item = (void*)((char*)item + (STRIDES)[DIM]);\
        }\
    }while(0)

//     do{\
//         long long __i,__j;\
//         long long *INDEX_SEQ=(long long*)LNMem_alloc(((DIM)+1)*sizeof(long long));\
//         for(__i=0; __i < (DIM); __i++){\
//             INDEX_SEQ[__i]=0;\
//         }\
//         size_t __axis_size = 1;\
//         for(__i=(DIM)+1; __i < (NDIM); __i++){\
//             __axis_size *= (DIMS)[__i];\
//         }\
// \
//         while(1){\
//             void *item = (void*)(VEC);\
//             for(__i=0; __i<(DIM); __i++){\
//                 item = (void*)((char*)item + (INDEX_SEQ[__i]*((STRIDES)[__i])));\
//             }\
//             CODE\
//             int finish=true;\
//             for(__i=0; __i < (DIM)+1; __i++){\
//                 if(((DIMS)[__i])-1!=INDEX_SEQ[__i]){\
//                     finish=false;\
//                     break;\
//                 }\
//             }\
//             if(finish)\
//                 break;\
//             INDEX_SEQ[(DIM)]++;\
//             for(__i=(DIM); __i>0; __i--){\
//                 if(INDEX_SEQ[__i]>((DIMS)[__i])){\
//                     for(__j=__i; __j >= 0; __j--){\
//                         INDEX_SEQ[__j]++;\
//                     }\
//                     INDEX_SEQ[__i] = 0;\
//                 }\
//             }\
//         }\
//     }while(0)

#define LN_ARRAY_APPLY_CONTIG(ARRAY, CODE)\
    LN_VECTOR_APPLY_CONTIG((ARRAY)->data, (ARRAY)->dimensions, (ARRAY)->strides, (ARRAY)->nd, CODE)\

#define LN_ARRAY_APPLY_D(ARRAY, DIM, CODE)\
    LN_VECTOR_APPLY_D((ARRAY)->data, (ARRAY)->strides, (ARRAY)->dimensions, DIM, (ARRAY)->nd, CODE)\

#define LN_VECTOR_APPLY2_CONTIG(VEC1, VEC2, DIMS1, DIMS2, STRIDES1, STRIDES2, NDIM, CODE)\
    long long __i,__j;\
    long long *INDEX_SEQ=(long long*)LNMem_alloc((NDIM)*sizeof(long long));\
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