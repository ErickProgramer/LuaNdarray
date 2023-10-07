#include "nlaux.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// repeats a printf
void nL_repprint(const char *txt, size_t q){
    for(size_t i = 0; i < q; i++)
        printf(txt);
}

size_t nL_getmaxalign(double *data, size_t data_len){
    char *str = NULL;
    sprintf(str, "%g", *data);

    size_t len, max = strlen(str);
    for(size_t i = 1; i < data_len; i++){
        sprintf(str, "%g", data[i]);
        len = strlen(str);
        if(len > max) max = len;
    }

    return max;
}
