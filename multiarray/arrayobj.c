#include <stdio.h>
#include <stdlib.h>

#include <lua.h>
#include <lauxlib.h>

#include "arrayobj.h"


Ndarray *newcNdarray(double *data, size_t *dims, size_t nd, int copy){
    Ndarray *arr = malloc(sizeof(Ndarray));
    if(!arr) return NULL;

    arr->nd = nd;

    arr->size = 1;
    for(size_t i = 0; i < nd; i++){
        arr->size *= dims[i];
    }

    // data
    if(copy){
        arr->data = malloc(sizeof(double) * arr->size);
        if(!arr->data) return NULL;

        for(size_t i = 0; i < arr->size; i++){
            arr->data[i] = data[i];
        }
    } else {
        arr->data = data;
    }

    // dimensions
    if(copy){
        arr->dimensions = malloc(sizeof(size_t) * nd);
        if(!arr->dimensions) return NULL;

        for(size_t i = 0; i < nd; i++){
            arr->dimensions[i] = dims[i];
        }
    } else {
        arr->dimensions = dims;
    }

    return arr;
};




static int nl_getShape(lua_State *L) {
    size_t pos = 1;

    lua_newtable(L);
    lua_pushvalue(L, -2);
    lua_remove(L,-3);

    while(lua_type(L, -1) == LUA_TTABLE){
        lua_pushinteger(L, pos);
        lua_pushinteger(L, luaL_len(L, -2));
        lua_settable(L, -4);
        pos++;

        lua_pushinteger(L, 1);
        lua_gettable(L, -2);
        lua_remove(L, -2);
    }

    lua_pushvalue(L, -2);
    lua_pushinteger(L, pos-1);

    return 2; // Return the shape and the ndim
}

// create a new array
static int nl_newCNdarray(lua_State *L){
    luaL_checktype(L, 1, LUA_TTABLE);
    luaL_checktype(L, 2, LUA_TTABLE);

    size_t i;
    size_t size = luaL_len(L, 1);
    size_t ndim = luaL_len(L, 2);

    size_t tmul = 1;
    for(i = 1; i <= ndim; i++){
        lua_rawgeti(L, 2, i);

        if(lua_type(L, -1) != LUA_TNUMBER){
            luaL_error(L, "shape[%d] not is a number", i);
        }

        tmul *= lua_tointeger(L, -1);

        lua_remove(L, -1);
    }

    if(tmul != size) luaL_error(L, "incompatible shape, the multiplication of the shape  %d != %d", size, tmul);

    size_t *dims = malloc(sizeof(size_t) * ndim);
    double *data = malloc(size * sizeof(double));


    if(!dims) luaL_error(L, "error on alloc the dimensions");
    if(!data) luaL_error(L, "error on alloc the data");

    for(i = 1; i <= ndim; i++){
        dims[i] = (size_t)lua_tointeger(L, i);
    }

    for(i = 1; i <= size; i++){
        data[i] = lua_tonumber(L, i);
    }

    Ndarray *res = lua_newuserdata(L, sizeof(Ndarray));
    if(!res) luaL_error(L, "error on allocate the Ndarray");

    luaL_getmetatable(L, "ndarray");
    lua_setmetatable(L, -2);

    res = newcNdarray(data, dims, ndim, 0);

    return 1;
}

