#include "arrayindex.h"
#include "arrayobj.h"
#include "nlaux.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*! index the `arr` in the index `idx`
 * @param arr the array you want to index
 * @param idx the desired index
 * @return a new Ndarray whose dimensions range from 1 to the length of `arr->dimensions` and the number of dimensions is `arr->nd - 1` and its data pointer points to another place in `arr->data` (does not create a copy)
 */
Ndarray *c_arrayindex(Ndarray *arr, int idx){
    Ndarray *res = malloc(sizeof(Ndarray));
    if(res == NULL)
        return NULL;

    res->nd = arr->nd - 1;
    if(arr->nd > 1)
        res->dimensions = arr->dimensions + 1;

    size_t i;
    res->size = 1;
    for(i = 1; i < arr->nd; i++)
        res->size *= arr->dimensions[i];

    res->data = arr->data + (idx * res->size);

    return res;
}

/*
 * does the same things as c_arrayindex but in a way that Lua understands
 */
int l_arrayindex(lua_State *L){
    int idxstart = luaLN_getIndexStart(L);

    Ndarray *arr = luaLN_checkndarray(L, 1);
    int idx = luaL_checkinteger(L, 2) - idxstart;

    Ndarray *res = c_arrayindex(arr, idx);
    if(res == NULL)
        luaL_error(L, "MemoryError: insufficient memory");

    luaLN_pushndarray(L, res);

    return 1;
}
