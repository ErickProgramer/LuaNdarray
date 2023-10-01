#ifndef NUMLUA_MULTIARRAY_ARRAYOBJ_H_
#define NUMLUA_MULTIARRAY_ARRAYOBJ_H_

#include <stdio.h>
#include <stdlib.h>

/* ndarray struct */
typedef struct Ndarray_fields{
    /* data of the ndarray */
    double *data;

    /* dimensions of the array */
    size_t *dimensions;

    /* number of dimensions */
    size_t nd;

    /* size of `data` (multiplications of the dimensions) */
    size_t size;
} Ndarray;

Ndarray *newcNdarray(double*, size_t*, size_t, int);

#define newNdarray(data, dims, nd) newcNdarray(data, dims, nd, 1)

#endif /* NUMLUA_MULTIARRAY_ARRAYOBJ_H_ */