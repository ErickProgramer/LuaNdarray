#ifndef LUANDARRAY_MULTIARRAY_ARRAYPRINT_H_
#define LUANDARRAY_MULTIARRAY_ARRAYPRINT_H_

#include <lua.h>
#include <lauxlib.h>

#include "arrayobj.h"

void c_arrayprint(Ndarray *arr, size_t indent_level);
int l_arrayprint(lua_State *L);

#endif /* LUANDARRAY_MULTIARRAY_ARRAYPRINT_H_ */