#ifndef LUANDARRAY_MULTIARRAY_ARRAYINIS_H_
#define LUANDARRAY_MULTIARRAY_ARRAYINIS_H_

#include "arrayobj.h"

#include <lua.h>
#include <lauxlib.h>

int l_full(lua_State*);
int l_zeros(lua_State*);
int l_ones(lua_State*);
int l_ndarray_by_table(lua_State*);
int table_flatten(lua_State*);
int aux_GetTableShape(lua_State*);
int aux_flatten(lua_State*);

#endif /* LUANDARRAY_MULTIARRAY_ARRAYINIS_H_ */