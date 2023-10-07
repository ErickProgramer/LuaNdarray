#include "arrayindex.h"
#include "arrayobj.h"

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
    res->dimensions = malloc(sizeof(size_t) * res->nd);
    if(res->dimensions == NULL)
        return NULL;

    size_t i;
    res->size = 1;
    for(i = 1; i < arr->nd; i++){
        res->dimensions[i-1] = arr->dimensions[i];
        res->size *= arr->dimensions[i];
    }

    res->data = arr->data + (idx * res->size);

    return res;
}

/*
 * does the same things as c_arrayindex but in a way that Lua understands
 */
int l_arrayindex(lua_State *L){
    Ndarray *arr = (Ndarray*)luaL_checkudata(L, 1, "ndarray");
    int idx = luaL_checkinteger(L, 2) - 1;

    Ndarray *res = c_arrayindex(arr, idx);
    lua_pushlightuserdata(L, (void*)res);

    luaL_getmetatable(L, "ndarray");
    lua_setmetatable(L, -2);


    return 1;
}
