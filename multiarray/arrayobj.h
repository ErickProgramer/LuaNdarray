#ifndef NUMLUA_MULTIARRAY_ARRAYOBJ_H_
#define NUMLUA_MULTIARRAY_ARRAYOBJ_H_

#include <stdio.h>
#include <stdlib.h>

#include <lua.h>
#include <lauxlib.h>

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

int l_newNdarray(lua_State*);
int l_NdarrayFree(lua_State*);

#endif /* NUMLUA_MULTIARRAY_ARRAYOBJ_H_ */