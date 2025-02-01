#ifndef LUANDARRAY_SRC_LUA_SRC_NDARRAY_H_
#define LUANDARRAY_SRC_LUA_SRC_NDARRAY_H_

#include<lua.h>
#include<lauxlib.h>

void ln_array_init(lua_State *L);

int ln_array_zeros(lua_State *L);
int ln_array_ones(lua_State *L);
int ln_array_arange(lua_State *L);
int ln_array_new(lua_State *L);

int ln_array__gc(lua_State *L);
int ln_array__tostring(lua_State *L);
int ln_array__add(lua_State *L);

#endif /*LUANDARRAY_SRC_LUA_SRC_NDARRAY_H_*/
