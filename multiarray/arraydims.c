#include "arraydims.h"
#include "arrayobj.h"
#include "nlaux.h"

#include <lua.h>
#include <lauxlib.h>
#include <string.h>

// alterate the shape of the Ndarray, is a auxiliar function used by `l_reshape` and others
void l_reshape_(lua_State *L, int arridx, int newshapeidx){
    Ndarray *arr = luaLN_checkndarray(L, 1);

    size_t new_ndim = luaL_len(L, newshapeidx);
    size_t *newshape = luaLN_malloc(L, sizeof(size_t) * new_ndim);

    size_t shapemul = 1;
    size_t i;
    for(i = 0; i < new_ndim; i++){
        lua_rawgeti(L, 2, i+1);

        newshape[i] = (size_t)lua_tonumber(L, -1);
        shapemul *= newshape[i];

        lua_remove(L, -1);
    }

    if(shapemul != arr->size){
        luaL_Buffer b;
        luaL_buffinit(L, &b);

        for (i = 1; i <= new_ndim; i++) {
            lua_rawgeti(L, 2, i);
            luaL_addstring(&b, lua_tostring(L, -1));
            if (i < new_ndim)
                luaL_addstring(&b, ",");
            lua_remove(L, -1);
        }

        luaL_pushresult(&b);
        const char *tstring = lua_tostring(L, -1);

        luaLN_error(L, "cannot reshape array of size %zu into shape {%s}", arr->size, tstring);
    }
    
    Ndarray *res = luaLN_newNdarray(L);

    res->data = luaLN_malloc(L, sizeof(double) * arr->size);
    memcpy(res->data, arr->data, sizeof(double) * arr->size);

    res->dimensions = newshape;
    res->nd = new_ndim;
    res->size = arr->size;
}

// does the same as `l_reshape_`, but receives only one argument, making it possible to send to Lua.
int l_reshape(lua_State *L){
    l_reshape_(L, 1, 2);
    return 1;
}

// returns an Ndarray with reversed dimensions
int l_transpose(lua_State *L){
    Ndarray *arr = luaLN_checkndarray(L, 1);

    // inverting the dimensions of the array by placing it in a Lua table and taking advantage of an existing function (l_reshape_)
    size_t i, l;
    lua_newtable(L);
    for(i = 1, l = arr->nd; i <= arr->nd; i++, l--){
        lua_pushinteger(L, arr->dimensions[l-1]);
        lua_rawseti(L, -2, i);
    }
    l_reshape_(L, 1, -1);

    return 1;
}

// transform a Ndarray in vector
int l_flatten(lua_State *L){
    Ndarray *arr = luaLN_checkndarray(L, 1);

    Ndarray *res = luaLN_newNdarray(L);

    res->nd = 1;
    res->dimensions = luaLN_malloc(L, sizeof(size_t));

    *res->dimensions = arr->size;
    res->size = arr->size;
    
    res->data = luaLN_malloc(L, sizeof(double) * res->size);

    memcpy(res->data, arr->data, sizeof(double) * arr->size);

    return 1;
}
