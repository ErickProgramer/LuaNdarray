#include <stdio.h>
#include <stdlib.h>

#include <lua.h>
#include <lauxlib.h>

#include "arrayobj.h"

// create a new array
int l_newNdarray(lua_State *L){
    // checking the types of the arguments
    luaL_checktype(L, 1, LUA_TTABLE);
    luaL_checktype(L, 2, LUA_TTABLE);

    // getting the size and ndim
    size_t arr_size = luaL_len(L, 1);
    size_t arr_ndim = luaL_len(L, 2);

    // create the userdata
    Ndarray *arr = (Ndarray*)lua_newuserdata(L, sizeof(Ndarray));

    // set the metatable in `arr`
    luaL_getmetatable(L, "ndarray");
    lua_setmetatable(L, -2);

    // initializing fields
    arr->nd = arr_ndim;
    arr->dimensions = malloc(sizeof(size_t) * arr_ndim);
    arr->size = arr_size;
    arr->data = malloc(sizeof(double) * arr_size);

    // checking whether allocations were successful
    if(arr->data == NULL)
        luaL_error(L, "error when allocating Ndarray data");
    if(arr->dimensions == NULL)
        luaL_error(L, "error when allocating Ndarray dimensions");

    // putting the numbers of the table to arr->dimensions
    for(size_t i = 0; i < arr_ndim; i++){
        lua_rawgeti(L, 2, i+1);
        arr->dimensions[i] = lua_tonumber(L, -1);
        lua_remove(L, -1);
    }

    // putting the numbers of the table to the arr->data
    for(size_t i = 0; i < arr->size; i++){
        lua_rawgeti(L, 1, i+1);
        arr->data[i] = lua_tonumber(L, -1);
        lua_remove(L, -1);
    }

    return 1; // returns the Ndarray
}

