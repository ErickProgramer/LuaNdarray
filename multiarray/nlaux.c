#include "nlaux.h"

#include <stdio.h>
#include <stdlib.h>

// repeats a printf
void nL_repprint(const char *txt, size_t q){
    for(size_t i = 0; i < q; i++)
        printf(txt);
}
