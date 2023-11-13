/*
 * arrayinis.c: has initializing functions, functions that create Ndarrays automatically, often needing to inform only the shape
 */

#include "arrayinis.h"
#include "nlaux.h"

#include <lua.h>
#include <lauxlib.h>
#include <math.h>

// fills an array with the specified value in the desired shape
int l_full(lua_State *L){
    if(lua_gettop(L) > 2)
        luaLN_error(L, "wrong number of arguments to 'full'");

    luaL_checktype(L, 1, LUA_TNUMBER);
    luaL_checktype(L, 2, LUA_TTABLE);

    size_t ndim = luaL_len(L, 2);

    Ndarray *res = luaLN_newNdarray(L);

    res->nd = ndim;
    res->dimensions = luaLN_malloc(L, sizeof(size_t) * ndim);
    res->size = 1;

    // creating the dimensions and size
    size_t i;
    for(i = 0; i < res->nd; i++){
        lua_rawgeti(L, 2, i+1);
        res->dimensions[i] = lua_tonumber(L, -1);
        res->size *= res->dimensions[i];
        lua_pop(L, 1);
    }

    // creating the data
    res->data = luaLN_malloc(L, sizeof(double) * res->size);
    for(i = 0; i < res->size; i++)
        res->data[i] = lua_tonumber(L, 1);

    return 1;
}

// initializes an Ndarray with zeros
int l_zeros(lua_State *L){
    if(lua_gettop(L) > 1)
        luaLN_error(L, "wrong number of arguments to 'zeros'");

    luaL_checktype(L, 1, LUA_TTABLE);
    Ndarray *res = luaLN_newNdarray(L);

    res->nd = luaL_len(L, 1);
    res->dimensions = luaLN_malloc(L, sizeof(size_t) * res->nd);

    res->size = 1;
    for(size_t i = 0; i < res->nd; i++){
        lua_rawgeti(L, 1, i+1);
        lua_Integer v = luaL_checkinteger(L, -1);
        if(v < 0)
            luaLN_error(L, "ValueError: in the index %zu, negative value.", i+luaLN_getIndexStart(L));
        res->dimensions[i] = (size_t)v;
        res->size *= res->dimensions[i];
        lua_pop(L, 1);
    }

    res->data = luaLN_calloc(L, sizeof(double), res->size);

    return 1;
}

// initializes an Ndarray with ones
int l_ones(lua_State *L){
    if(lua_gettop(L) > 1)
        luaLN_error(L, "wrong number of arguments to 'ones'");

    lua_pushinteger(L, 1);
    lua_pushvalue(L, -2);
    lua_remove(L, -3);
    l_full(L);

    return 1;
}
