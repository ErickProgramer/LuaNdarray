#ifndef NUMLUA_MULTIARRAY_ARRAYINDEX_H_
#define NUMLUA_MULTIARRAY_ARRAYINDEX_H_

#include <lua.h>
#include <lauxlib.h>

#include "arrayobj.h"

Ndarray *c_arrayindex(Ndarray*, int);
int l_arrayindex(lua_State*);

#endif /* NUMLUA_MULTIARRAY_ARRAYINDEX_H_ */