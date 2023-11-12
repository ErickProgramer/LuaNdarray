#ifndef LUANDARRAY_MULTIARRAY_ARRAYINIS_H_
#define LUANDARRAY_MULTIARRAY_ARRAYINIS_H_

#include "arrayobj.h"

#include <lua.h>
#include <lauxlib.h>

int l_full(lua_State *L);
int l_zeros(lua_State *L);
int l_ones(lua_State *L);

#endif /* LUANDARRAY_MULTIARRAY_ARRAYINIS_H_ */