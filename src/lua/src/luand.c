#include<stdio.h>
#include<stdlib.h>

#include<luandarray.h>
#include "luand.h"
#include "conf.h"

void ln_debug_stack(lua_State *L);

void *luaND_alloc(lua_State *L, size_t bytes){
    void *block = LNMem_alloc(bytes);
    if(!block){
        luaL_error(L, LNError_Get());
        return NULL;
    }
    return block;
}

void *luaND_realloc(lua_State *L, void *ptr, size_t bytes){
    void *r = LNMem_realloc(ptr, bytes);
    if(!r){
        luaL_error(L, LNError_Get());
        return NULL;
    }
}


void luaND_pushndarray(lua_State *L, Ndarray *arr){
    Ndarray **res = (Ndarray**)lua_newuserdata(L, sizeof(Ndarray*));
    *res = arr;
    luaL_getmetatable(L, LN_NDARRAY_MT);
    lua_setmetatable(L, -2);
}

void luaND_pushdtypep(lua_State *L, const LNTypeDescr *dtype){
    LNTypeDescr **ud = (LNTypeDescr**)lua_newuserdata(L, sizeof(LNTypeDescr*));
    *ud = (LNTypeDescr*)dtype;

    luaL_getmetatable(L, LN_DTYPE_MT);
    lua_setmetatable(L, -2);
}

void luaND_pushdtype(lua_State *L, LNDTypes dtype){
    luaND_pushdtypep(L, LNDType_GetFromID(dtype));
}

// a copy of the function luaL_setfuncs
void luaND_setfuncs (lua_State *L, const luaL_Reg *l, int nup) {
    luaL_checkstack(L, nup, "too many upvalues");
    for (; l->name != NULL; l++) {  /* fill the table with given functions */
        if (l->func == NULL)  /* placeholder? */
            lua_pushboolean(L, 0);
    else {
      int i;
      for (i = 0; i < nup; i++)  /* copy upvalues to the top */
        lua_pushvalue(L, -nup);
      lua_pushcclosure(L, l->func, nup);  /* closure with those upvalues */
    }
    lua_setfield(L, -(nup + 2), l->name);
  }
  lua_pop(L, nup);  /* remove upvalues */
}

int luaND_isndarray(lua_State *L, int arg){
    if(lua_isuserdata(L, arg)){
        lua_getmetatable(L, arg);
        luaL_getmetafield(L, -1, "__name");
        lua_pushstring(L, LN_NDARRAY_MT);
        int is = lua_rawequal(L, -2, -1);
        lua_pop(L, 1);
        return is;
    }
    return 0;
}

int luaND_isdtype(lua_State *L, int arg){
    if(lua_isuserdata(L, arg)){
        luaL_getmetafield(L, arg, "__name");
        lua_pushstring(L, LN_DTYPE_MT);
        return lua_rawequal(L, -2, -1);
    }
    return 0;
}


int luaND_type(lua_State *L, int arg){
    if(luaND_isndarray(L, arg)){
        return LUAND_TNDARRAY;
    }

    return lua_type(L, arg);
}

int luaND_newmetatable(lua_State *L, const char *name){
    luaL_getmetatable(L, name);
    if(!lua_isnil(L, -1)){
        lua_pop(L, 1);
        return 0;
    }

    lua_newtable(L);
    lua_pushstring(L, "__name");
    lua_pushstring(L, name);
    lua_settable(L, -3);

    lua_setfield(L, LUA_REGISTRYINDEX, name);

    // lua_pop(L, 2);

    return 1;
}

int luaND_testudata(lua_State *L, int arg, const char *uname){
    if(lua_isuserdata(L, arg) && lua_getmetatable(L, arg)){
        luaL_getmetatable(L, uname);
        int eq = lua_rawequal(L, -1, -2);
        lua_pop(L, 2);
        return eq;
    }
    return 0;
}

void *luaND_checkudata(lua_State *L, int arg, const char *ud){
    luaND_argcheck(L, luaND_testudata(L, arg, ud), arg, "%s expected", ud);
    return lua_touserdata(L, arg);
}

// LuaNdarray's argcheck function
int luaND_argcheck(lua_State *L, int cond, int arg, const char *fmt, ...){
    if(!cond){
        va_list ap;
        va_start(ap, fmt);
        lua_pushvfstring(L, fmt, ap);

        luaL_argerror(L, arg, lua_tostring(L, -1));
        lua_pop(L, 1);
        return 0;
    }
    return 1;
}
