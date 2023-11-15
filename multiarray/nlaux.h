#ifndef LUANDARRAY_MULTIARRAY_NLAUX_H_
#define LUANDARRAY_MULTIARRAY_NLAUX_H_

#include "arrayobj.h"
#include "lnconf.h"

#include <lua.h>
#include <lauxlib.h>

#include <stdio.h>
#include <stdlib.h>

// old functions
void nL_repprint(const char*, size_t);
size_t nL_getmaxalign(double*, size_t);

/* luaLN_* functions */

void *luaLN_mallocd(lua_State *L, size_t bytes, int detailed);
void luaLN_MallocdError(lua_State *L, size_t bytes, int detailed);
void *luaLN_callocd(lua_State *L, size_t q, size_t bytes, int detailed);

Ndarray *luaLN_newNdarraym(lua_State *L, int setmetatable);
void luaLN_checkargcount(lua_State *L, const char *fname, size_t limit);
void luaLN_pushndarraym(lua_State *L, Ndarray *arr, int setmetatable);

int luaLN_checkboolean(lua_State *L, int arg);
int luaLN_getIndexStart(lua_State *L);
size_t luaLN_adjustpos(lua_State *L, lua_Integer pos, size_t limit, size_t axis);

void luaLN_setfuncs(lua_State *L, const luaL_Reg *funcs, int tablepos);
void luaLN_newlib(lua_State *L, const luaL_Reg *lib);

void luaLN_error(lua_State *L, const char *fmt, ...);

Ndarray *luaLN_casttoNdarray(lua_State *L, int vpos);

void luaLN_NdarrayFree(Ndarray *arr);
/*********************/

/* luaLN_* macros */

#define luaLN_malloc(L, bytes) luaLN_mallocd(L, bytes, 1)
#define luaLN_MallocError(L, bytes) luaLN_MallocdError(L, bytes, 1)
#define luaLN_calloc(L, q, bytes) luaLN_callocd(L, q, bytes, 1)
#define luaLN_pushndarray(L, arr) luaLN_pushndarraym(L, arr, 1)
#define luaLN_checkndarray(L, arg) ((Ndarray*)luaL_checkudata(L, (arg), NDARRAY_USERDATA_NAME))
#define luaLN_newNdarray(L) luaLN_newNdarraym(L, 1)
#define luaLN_adjustargpos(L, arg, limit, axis) luaLN_adjustpos(L, luaL_checkinteger(L, (arg)), limit, axis);

/******************/

#endif /* LUANDARRAY_MULTIARRAY_NLAUX_H_ */