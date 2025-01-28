#include<stdio.h>
#include<stdlib.h>

#include "error.h"
#include "arrayobj.h"
#include "buffer.h"
#include "arrayindex.h"

static void LNArray_RecursiveToString(LNBuffer *b, Ndarray *arr, size_t indent_level){
    Ndarray *aux = LNArray_Alloc();
    if(!aux)
        return;
    LNBuff_addchar(b, '{');
    if(LNError_Ocurred()){
        return;
    }

    size_t i, j;
    for(i = 0; i < LNArray_LEN(arr); i++){
        LNArray_Index_(aux, arr, i);
        if(LNError_Ocurred()){
            return;
        }
        if(LNArray_IsScalar(aux)){
            LNBuff_addtype(b, aux->data, aux->dtype);
            if(LNError_Ocurred()){
                return;
            }
            if(i < LNArray_LEN(arr)-1){
                LNBuff_addchar(b, '\t');
                if(LNError_Ocurred()){
                    return;
                }
            }
        } else{
            if(i > 0){
                for(j = 0; j < indent_level; j++){
                    LNBuff_addchar(b, ' ');
                    if(LNError_Ocurred()){
                        return;
                    }
                }
            }
            LNArray_RecursiveToString(b, aux, indent_level+1);
            if(i < LNArray_LEN(arr)-1){
                for(j = 0; j < LNArray_NDIM(aux); j++){
                    LNBuff_addchar(b, '\n');
                    if(LNError_Ocurred()){
                        return;
                    }
                }
            }
        }
    }
    free(aux);
    LNBuff_addchar(b, '}');
    if(LNError_Ocurred()){
        return;
    }
}

char *LNArray_toString_(LNBuffer *b, Ndarray *arr, const char *prefix, const char *sufix){
    if(!b){
        b = LNBuff_alloc();
        if(!b){
            return NULL;
        }
        LNBuff_Init(b);
    }

    LNBuff_addstring(b, prefix);
    if(LNArray_IsScalar(arr)){
        LNBuff_addtype(b, arr->data, arr->dtype);
    } else if(LNArray_IsEmpty(arr)){
        LNBuff_addstring(b, "{}, shape={");
        size_t i;
        for(i = 0; i < arr->nd; i++){
            LNBuff_addfstring(b, "%zu", LNArray_DIMS(arr)[i]);
            if(i < arr->nd-1){
                LNBuff_addstring(b, ", ");
            }
        }
        LNBuff_addchar(b, '}');
    } else {
        LNArray_RecursiveToString(b, arr, strlen(prefix)+1);
    }
    LNBuff_addfstring(b, ", dtype=%s", arr->dtype->name);
    LNBuff_addstring(b, sufix);

    return LNBuff_ADDR(b);
}

char *LNArray_toString(Ndarray *arr, const char *prefix, const char *sufix){
    LNBuffer b;
    LNBuff_Init(&b);
    LNArray_toString_(&b, arr, prefix, sufix);
    return LNBuff_ADDR(&b);
}

void LNArray_Print(Ndarray *arr, const char *prefix, const char *sufix){
    printf("%s", LNArray_toString(arr, prefix, sufix));
}
