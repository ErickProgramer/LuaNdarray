#include "arraydims.h"
#include "arrayobj.h"

#include <lua.h>
#include <lauxlib.h>
#include <string.h>

void l_reshape_(lua_State *L, int arridx, int newshapeidx){
    Ndarray *arr = (Ndarray*)luaL_checkudata(L, 1, "ndarray");

    size_t new_ndim = luaL_len(L, newshapeidx);
    size_t *newshape = malloc(sizeof(size_t) * new_ndim);
    if(newshape == NULL) luaL_error(L, "error on allocate the shape");

    size_t i;
    for(i = 0; i < new_ndim; i++){
        lua_rawgeti(L, 2, i+1);
        newshape[i] = (size_t)lua_tonumber(L, -1);
        lua_remove(L, -1);
    }

    size_t shapemul = 1;
    for(i = 0; i < new_ndim; i++)
        shapemul *= newshape[i];

    if(shapemul != arr->size){
        char tstring[101] = {'\0'};
        for(i = 1; i <= new_ndim; i++){
            lua_rawgeti(L, 2, i);
            strcat(tstring, lua_tostring(L, -1));
            if(i < new_ndim)
                strcat(tstring, ",");
            lua_remove(L, -1);
        }
        luaL_error(L, "cannot reshape array of size %d into shape {%s}", arr->size, tstring);
    }
    
    Ndarray *res = lua_newuserdata(L, sizeof(Ndarray));

    luaL_getmetatable(L, "ndarray");
    lua_setmetatable(L, -2);
    
    res->data = malloc(sizeof(double) * arr->size);
    for(i = 0; i < arr->size; i++)
        res->data[i] = arr->data[i];

    res->dimensions = newshape;
    res->nd = new_ndim;
    res->size = arr->size;
    // return 1;
}

int l_reshape(lua_State *L){
    l_reshape_(L, 1, 2);
    return 1;
    // Ndarray *arr = luaL_checkudata(L, 1, "ndarray");

    // size_t new_ndim = luaL_len(L, 2);
    // size_t *newshape = malloc(sizeof(size_t) * new_ndim);
    // if(newshape == NULL) luaL_error(L, "error on allocate the shape");

    // size_t i;
    // for(i = 0; i < new_ndim; i++){
    //     lua_rawgeti(L, 2, i+1);
    //     newshape[i] = (size_t)lua_tonumber(L, -1);
    //     lua_remove(L, -1);
    // }

    // size_t shapemul = 1;
    // for(i = 0; i < new_ndim; i++)
    //     shapemul *= newshape[i];
    
    // if(shapemul != arr->size){
    //     char tstring[101];
    //     for(i = 1; i <= new_ndim; i++){
    //         lua_rawgeti(L, 2, i);
    //         strcat(tstring, lua_tostring(L, -1));
    //         if(i < new_ndim)
    //             strcat(tstring, ",");
    //         lua_remove(L, -1);
    //     }
    //     luaL_error(L, "cannot reshape array of size %d into shape {%s}", arr->size, tstring);
    // }
    
    // Ndarray *res = lua_newuserdata(L, sizeof(Ndarray));

    // luaL_getmetatable(L, "ndarray");
    // lua_setmetatable(L, -2);
    
    // res->data = malloc(sizeof(double) * arr->size);
    // for(i = 0; i < arr->size; i++)
    //     res->data[i] = arr->data[i];

    // res->dimensions = newshape;
    // res->nd = new_ndim;
    // res->size = arr->size;

    // return 1;
}

int l_transpose(lua_State *L){
    Ndarray *arr = (Ndarray*)luaL_checkudata(L, 1, "ndarray");
    size_t i, l;
    lua_newtable(L);
    for(i = 1, l = arr->nd; i <= arr->nd; i++, l--){
        lua_pushinteger(L, arr->dimensions[l-1]);
        lua_rawseti(L, -2, i);
    }
    l_reshape_(L, 1, -1);
    return 1;
}

int l_flatten(lua_State *L){
    Ndarray *arr = (Ndarray*)luaL_checkudata(L, 1, "ndarray");

    lua_newtable(L);
    lua_pushinteger(L, arr->size);

    l_reshape_(L, 1, 2);

    return 1;
}