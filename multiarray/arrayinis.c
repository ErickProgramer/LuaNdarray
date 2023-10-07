#include "arrayinis.h"

#include <lua.h>
#include <lauxlib.h>

int l_full(lua_State *L){
    luaL_checktype(L, 1, LUA_TNUMBER);
    luaL_checktype(L, 2, LUA_TTABLE);

    size_t ndim = luaL_len(L, 2);

    Ndarray *res = (Ndarray*)lua_newuserdata(L, sizeof(Ndarray));

    luaL_getmetatable(L, "ndarray");
    lua_setmetatable(L, -2);
    
    res->nd = ndim;
    res->dimensions = malloc(sizeof(size_t) * ndim);
    res->size = 1;

    size_t i;
    for(i = 0; i < res->nd; i++){
        lua_rawgeti(L, 2, i+1);
        res->dimensions[i] = lua_tonumber(L, -1);
        res->size *= res->dimensions[i];
        lua_remove(L, -1);
    }

    res->data = malloc(sizeof(double) * res->size);
    for(i = 0; i < res->size; i++)
        res->data[i] = lua_tonumber(L, 1);

    return 1;
}

int l_zeros(lua_State *L){
    luaL_checktype(L, 1, LUA_TTABLE);
    Ndarray *res = (Ndarray*)lua_newuserdata(L, sizeof(Ndarray));

    luaL_getmetatable(L, "ndarray");
    lua_setmetatable(L, -2);

    res->nd = luaL_len(L, 1);
    res->dimensions = malloc(sizeof(size_t) * res->nd);

    if(res->dimensions == NULL)
        luaL_error(L, "error ocurred on allocate the dimensins.");

    res->size = 1;
    for(size_t i = 0; i < res->nd; i++){
        lua_rawgeti(L, 1, i+1);
        lua_Integer v = luaL_checkinteger(L, -1);
        if(v < 0)
            luaL_error(L, "dimensions error in the index %d, negative value.", i+1);
        res->dimensions[i] = (size_t)v;
        res->size *= res->dimensions[i];
        lua_remove(L, -1);
    }

    res->data = calloc(sizeof(double), res->size);

    if(res->data == NULL)
        luaL_error(L, "error on allocate the data.");

    return 1;
}

int l_ndarray_by_table(lua_State *L){
    Ndarray *res = (Ndarray*)lua_newuserdata(L, sizeof(Ndarray));

    luaL_getmetatable(L, "ndarray");
    lua_setmetatable(L, -2);

    res->size = 1;
    size_t pos = 1;
    lua_newtable(L);
    lua_pushvalue(L, 1);
    while(lua_type(L, -1) == LUA_TTABLE){
        lua_pushinteger(L, luaL_len(L, -1));
        res->size *= luaL_len(L, -2); 
        lua_rawseti(L, -3, pos);
        lua_rawgeti(L, -1, 1);
        lua_remove(L, -2);
        pos++;
    }
    lua_remove(L,-1);

    res->nd = luaL_len(L, -1);
    res->dimensions = malloc(sizeof(size_t) * res->nd);
    res->data = malloc(sizeof(double) * res->size);

    for(size_t i = 0; i < res->nd; i++){
        lua_rawgeti(L, -1, i+1);
        res->dimensions[i] = lua_tonumber(L, -1);
        lua_remove(L, -1);
    }

    lua_remove(L,-1);

    return 1;
}


