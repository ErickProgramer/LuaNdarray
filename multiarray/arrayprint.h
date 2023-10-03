#ifndef NUMLUA_MULTIARRAY_ARRAYPRINT_H_
#define NUMLUA_MULTIARRAY_ARRAYPRINT_H_

#include <lua.h>
#include <lauxlib.h>

#include "arrayobj.h"

void c_arrayprint(Ndarray*);
int l_arrayprint(lua_State*);

#endif