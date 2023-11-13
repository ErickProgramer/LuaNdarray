#ifndef LUANDARRAY_MULTIARRAY_CONVERTS_H_
#define LUANDARRAY_MULTIARRAY_CONVERTS_H_

#include <stdio.h>
#include <stdlib.h>

#include <lauxlib.h>

#include "arrayobj.h"

void c_ndarraytostring(luaL_Buffer *b, Ndarray *arr, size_t indent_level, const char *prefix, const char *sufix);
int l_ndarraytostring(lua_State *L);

#endif /* LUANDARRAY_MULTIARRAY_CONVERTS_H_ */