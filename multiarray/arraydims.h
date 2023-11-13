#ifndef LUANDARRAY_MULTIARRAY_ARRAYDIMS_H_
#define LUANDARRAY_MULTIARRAY_ARRAYDIMS_H_

#include <lua.h>
#include <lauxlib.h>

void l_reshape_(lua_State *L, int arridx, int newshapeidx);
int l_reshape(lua_State *L);
int l_transpose(lua_State *L);
int l_flatten(lua_State *L);

#endif /* LUANDARRAY_MULTIARRAY_ARRAYDIMS_H_ */