#ifndef LUANDARRAY_SRC_LUA_LUAND_H_
#define LUANDARRAY_SRC_LUA_LUAND_H_

#include<stdio.h>
#include<stdlib.h>

#include<luandarray.h>
#include<lua.h>
#include<lauxlib.h>

void *luaND_alloc(lua_State *L, size_t bytes);
void *luaND_realloc(lua_State *L, void *ptr, size_t _size);

Ndarray *luaND_newndarray(lua_State *L);


void luaND_pushndarray(lua_State *L, Ndarray *arr);
void luaND_pushdtypep(lua_State *L, const LNTypeDescr *dtype);
void luaND_pushdtype(lua_State *L, LNDTypes dtype);

void luaND_pushfunsp(lua_State *L, const luaL_Reg *funs, int to);

#define luaND_pushfuns(L, funs) (lua_newtable(), luaND_pushfunsp(L, -1))

int luaND_isndarray(lua_State *L, int arg);
int luaND_isdtype(lua_State *L, int arg);
int luaND_type(lua_State *L, int arg);

Ndarray *luaND_optndarray(lua_State *L, int arg, Ndarray *d);

#define luaND_checkndarray(L, arg) (*(Ndarray**)luaL_checkudata((L), (arg), LN_NDARRAY_MT))
#define luaND_checkdtype(L, arg) (*(const LNTypeDescr**)luaL_checkudata((L), (arg), LN_DTYPE_MT))


#define luaND_optdtype(L, arg, dtype) (lua_isnoneornil((L), (arg)) ? (dtype) : luaND_checkdtype(L, arg))
#define luaND_optndarray(L, arg, arr) (lua_isnoneornil((L), (arg)) ? (arr) : luaND_checkndarray(L, arg))

int luaND_newmetatable(lua_State *L, const char *name);

int luaND_testudata(lua_State *L, int arg, const char *uname);
void *luaND_checkudata(lua_State *L, int arg, const char *ud);

int luaND_argcheck(lua_State *L, int cond, int arg, const char *fmt, ...);

void luaND_setfuncs(lua_State *L, const luaL_Reg *l, int nup);

#define luaND_newndarray(L) ((Ndarray*)lua_newuserdata(L, sizeof(Ndarray)))

#define luaND_newlib(L, l) (luaND_newlibtable(L,l), luaND_setfuncs(L,l,0))
#define luaND_newlibtable(L,l)   \
  lua_createtable(L, 0, sizeof(l)/sizeof((l)[0]) - 1)

#if LUA_VERSION_NUM == 501
#define luaND_len(L, obj) lua_objlen(L, obj)
#else
#define luaND_len(L, obj) luaL_len(L, obj)
#endif

#define luaND_opttable(L, arg, extramsg) (luaL_argcheck(L, lua_istable(L, arg) || lua_isnoneornil(L, arg), numarg, extramsg))

#define LUAND_TNDARRAY 9

#endif /* LUANDARRAY_SRC_LUA_LUAND_H_ */
