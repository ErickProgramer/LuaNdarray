#ifndef LUANDARRAY_MULTIARRAY_ARRAYPRINT_H_
#define LUANDARRAY_MULTIARRAY_ARRAYPRINT_H_

#include <lua.h>
#include <lauxlib.h>

#include "arrayobj.h"

void c_arrayprint(Ndarray*);
int l_arrayprint(lua_State*);

#endif /* LUANDARRAY_MULTIARRAY_ARRAYPRINT_H_ */