#ifndef NUMLUA_MULTIARRAY_CONVERTS_H_
#define NUMLUA_MULTIARRAY_CONVERTS_H_

#include <stdio.h>
#include <stdlib.h>

#include <lauxlib.h>

#include "arrayobj.h"

void c_ndarraytostring(luaL_Buffer*, Ndarray*, size_t, const char*, const char*);
int l_ndarraytostring(lua_State*);

#endif /* NUMLUA_MULTIARRAY_CONVERTS_H_ */