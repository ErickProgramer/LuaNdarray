#include<stdio.h>
#include<stdlib.h>

#include<lua.h>
#include<lauxlib.h>
#include<luandarray.h>

#include "luand.h"
#include "conf.h"

int ln_debug_stack(lua_State *L);

// __tostring meta method
static int ln_dtype__tostring(lua_State *L){
    const LNTypeDescr *dtype = luaND_checkdtype(L, 1);
    lua_pushfstring(L, "dtype(\"%s\")", dtype->name);
    return 1;
}

static int ln_dtype__index(lua_State *L){
    const LNTypeDescr *dtype = luaND_checkdtype(L, 1);

    lua_pushstring(L, "alignment");
    if(lua_rawequal(L, -1, -2)){
        lua_pushinteger(L, (lua_Integer)dtype->alignment);        
        return 1;
    }
    lua_pop(L, 1);

    lua_pushstring(L, "itemsize");
    if(lua_rawequal(L, -1, -2)){
        lua_pushinteger(L, (lua_Integer)dtype->itemsize);
        return 1;
    }
    lua_pop(L, 1);

    lua_pushstring(L, "name");
    if(lua_rawequal(L, -1, -2)){
        lua_pushstring(L, dtype->name);
        return 1;
    }
    lua_pop(L, 1);

    lua_pushnil(L);
    return 1;
}

static int ln_dtype__eq(lua_State *L){
    if(luaND_isdtype(L, 1) && luaND_isdtype(L, 2)){
        const LNTypeDescr *dtype1 = luaND_checkdtype(L, 1);
        const LNTypeDescr *dtype2 = luaND_checkdtype(L, 2);
        lua_pushboolean(L, dtype1 != NULL && dtype1->id == dtype2->id);
    } else {
        lua_pushboolean(L, 0);
    }
}

static const luaL_Reg dtype_meta[] = {
    {"__tostring", ln_dtype__tostring},
    {"__index", ln_dtype__index},
    // {"__eq", ln_dtype__eq},
    {NULL, NULL}
};

void ln_makemetatable(lua_State *L){
    luaL_newmetatable(L, LN_DTYPE_MT);
    lua_pushboolean(L, 1);
    lua_setfield(L, -2, "__metatable");
    luaND_setfuncs(L, dtype_meta, 0);
    lua_pop(L, 1);
}

static void ln_setdtype(lua_State *L, LNDTypes id){
    const LNTypeDescr *dtype = LNDType_GetFromID(id);
    lua_pushstring(L, dtype->name);
    luaND_pushdtypep(L, dtype);
    lua_settable(L, -3);
}

static void ln_setdtypes(lua_State *L){
    int i;
    for(i = 0; i < LN_NUMTYPES; i++){
        ln_setdtype(L, i);
    }
}

void ln_dtype_init(lua_State *L){
    ln_makemetatable(L);
    ln_setdtypes(L);
}