#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <lua.h>
#include <lauxlib.h>

#include "arrayobj.h"
#include "nlaux.h"
#include "arrayindex.h"

// create a new array
int l_newNdarray(lua_State *L){
    // checking the types of the arguments
    luaL_checktype(L, 1, LUA_TTABLE);
    luaL_checktype(L, 2, LUA_TTABLE);

    // getting the size and ndim
    size_t arr_size = luaL_len(L, 1);
    size_t arr_ndim = luaL_len(L, 2);

    // create the userdata
    Ndarray *arr = luaLN_newNdarray(L);

    // initializing fields
    arr->nd = arr_ndim;
    arr->dimensions = luaLN_malloc(L, sizeof(size_t) * arr_ndim);
    arr->size = arr_size;
    arr->data = luaLN_malloc(L, sizeof(double) * arr_size);

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

// free a Ndarray
int l_NdarrayFree(lua_State *L){
    Ndarray *arr = luaLN_checkndarray(L, 1);

    if(arr){
        free(arr->data);
        free(arr->dimensions);
        free(arr);
    }

    return 0;
}

// Get the shape of the Ndarray
int l_getshape(lua_State *L){
    Ndarray *arr = luaLN_checkndarray(L, 1);

    lua_newtable(L);
    for(size_t i = 0; i < arr->nd; i++){
        lua_pushinteger(L, arr->dimensions[i]);
        lua_rawseti(L, -2, i+1);
    }

    return 1;
}

// Get the size of the Ndarray
int l_getsize(lua_State *L){
    Ndarray *arr = luaLN_checkndarray(L, 1);

    lua_pushinteger(L, arr->size);

    return 1;
}

// convert a Ndarray to a Lua scalar
int l_item(lua_State *L){
    Ndarray *arr = luaLN_checkndarray(L, 1);

    switch(lua_gettop(L)){
        case 1:{
            if(arr->size != 1)
                luaL_error(L, "can only convert an array of size 1 to a Lua scalar");
            
            lua_pushnumber(L, (*arr->data));
            break;
        }
        
        case 2:{
            lua_Integer pos = luaLN_adjustargpos(L, 2, arr->size, 0);

            lua_pushnumber(L, arr->data[pos]);
            break;
        }

        default:
            luaLN_error(L, "wrong number of arguments to 'item'");
            break;
    }

    return 1;
}

// __len meta method
int l_len(lua_State *L){
    Ndarray *arr = luaLN_checkndarray(L, 1);
    if(arr->nd == 0)
        luaL_error(L, "Can't get length a 0-dimensional Ndarray");

    lua_pushinteger(L, (lua_Integer)arr->dimensions[0]);

    return 1;
}

// __index meta method
int l_index(lua_State *L){
    Ndarray *arr = luaLN_checkndarray(L, 1);
    if(lua_type(L, 2) == LUA_TNUMBER){
        size_t idx = luaLN_adjustargpos(L, 2, arr->dimensions[0], 0);

        Ndarray *res = c_arrayindex(arr, idx);
        luaLN_pushndarray(L, res);

        return 1;
    } else if(lua_type(L, 2) == LUA_TSTRING){
        const char *idx = luaL_checkstring(L, 2);
        if(strcmp(idx, "ndim") == 0){
            lua_pushinteger(L, arr->nd);
            return 1;
        }
        else if(strcmp(idx, "shape") == 0){
            l_getshape(L);
            return 1;
        }
        else if(strcmp(idx, "size") == 0){
            l_getsize(L);
            return 1;
        }
    }
    
    luaL_getmetatable(L, NDARRAY_USERDATA_NAME);
    lua_pushvalue(L, 2);
    lua_rawget(L, -2);

    return 1;
}

int l_type(lua_State *L){
}

int l_IndexStartZero(lua_State *L){
    luaLN_checkboolean(L, 1);
    lua_setglobal(L, "__INDEX_START_IN_ZERO");
    return 0;
}

int l_GetIndexStart(lua_State *L){
    lua_getglobal(L, "__INDEX_START_IN_ZERO");
    int in_zero = luaLN_checkboolean(L, -1);
    if(in_zero)
        lua_pushinteger(L, 0);
    else
        lua_pushinteger(L, 1);

    return 1;
}
