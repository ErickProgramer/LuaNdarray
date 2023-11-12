#ifndef LUANDARRAY_MULTIARRAY_ARRAYOBJ_H_
#define LUANDARRAY_MULTIARRAY_ARRAYOBJ_H_

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

int l_newNdarray(lua_State *L);
int l_NdarrayFree(lua_State *L);
int l_getshape(lua_State *L);
int l_getsize(lua_State *L);
int l_item(lua_State *L);
int l_len(lua_State *L);
int l_index(lua_State *L);
int l_IndexStartZero(lua_State *L);
int l_GetIndexStart(lua_State *L);

#endif /* LUANDARRAY_MULTIARRAY_ARRAYOBJ_H_ */