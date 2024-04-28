#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>

#include "luandarray.h"

#define LNArray_NewInt8(data, dims, nd) LNArray_New((void*)(data), dims, nd, LNInt8);

/*
>>> np.ndarray(shape=(10,10),dtype=np.int8,strides=(5,6))
array([[ 79, 105, 101,  32, 111, 111,  32,  99, 116, 114],
       [  0,   0,   0,   0,   0,   0,   0,   0,   0,   0],
       [116,  97, -29, 112, 101, 110, 110,  97, 111, 114],
       [  0,   0,   0,   0,   0,   0,   0,   0,   0,   0],
       [110,  32, 100, 101, 111, 114,  32,  97, 117, 111],
       [  0,   0,   0,   0,   0,   0,   0,   0,   0,   0],
       [111,  32,  99, 116, 114,  32, 113, 118, 101, 101],
       [  0,   0,   0,   0,   0,   0,   0,   0,   0,   0],
       [110, 110,  97, 111, 114, 105,  32, 112, 105,  99],
       [  0,   0,   0,   0,   0,   0,   0,   0,   0,   0]], dtype=int8)
*/

int main(){
    require_luandarray();

    size_t dims[]={10,10};
    int8_t data[10*10];

    size_t i;
    for(i=0;i<10*10;i++)
        data[i]=i;

    Ndarray *arr = LNArray_New(data,dims,NULL,2,LNInt8);
    arr->strides[0]=1;
    arr->strides[1]=1;
    LNArray_Print(arr,"Tensor(",")\n");

    return 0;
}
