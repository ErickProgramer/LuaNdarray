#include<stdio.h>
#include<stdlib.h>

#include<luandarray.h>
#include<lua.h>
#include<lauxlib.h>
#include"luand.h"
#include "ndarray.h"
#include "conf.h"

void ln_debug_stack(lua_State *L){
    int i;
    for(i = 1; i <= lua_gettop(L); i++){
        printf("%s\t%s\n", lua_tostring(L, i), luaL_typename(L, i));
    }
}

void ln_dtype_init(lua_State *L);
void ln_ndarray_init(lua_State *L);


static int ln_type(lua_State *L){
    if(luaND_isdtype(L, 1)){
        lua_pushstring(L, "dtype");
    } else if(luaND_isndarray(L, 2)){
        lua_pushstring(L, "ndarray");
    } else{
        lua_pushstring(L, luaL_typename(L, 1));
    }
    return 1;
}


int ln_setdefaulttype(lua_State *L){
    if(lua_gettop(L) != 1){
        luaL_error(L, "wrong number of arguments to 'setdefaulttype'");
    }

    const LNTypeDescr *dtype = luaND_checkdtype(L, 1);
    LNDType_setdefault(dtype);
    return 1;
}

int ln_getdefaulttype(lua_State *L){
    luaND_pushdtypep(L, LNDType_getdefault());
    return 1;
}

static const luaL_Reg ln[] = {
    {"arange", ln_array_arange},
    {"ndarray", ln_array_new},
    {"ones", ln_array_ones},
    {"zeros", ln_array_zeros},
    {"type", ln_type},
    {"setdefaulttype", ln_setdefaulttype},
    {"getdefaulttype", ln_getdefaulttype},
    {NULL, NULL}
};

int luaopen_luandarray(lua_State *L){
    // loading LuaNdarray
    require_luandarray();

    luaND_newlibtable(L, ln);

    // Initializing dtypes
    ln_dtype_init(L);

    // initializing the Ndarray object
    ln_array_init(L);


    luaND_setfuncs(L, ln, 0);

    return 1;
}
