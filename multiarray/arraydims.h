#ifndef NUMLUA_MULTIARRAY_ARRAYDIMS_H_
#define NUMLUA_MULTIARRAY_ARRAYDIMS_H_

#include <lua.h>
#include <lauxlib.h>

void l_reshape_(lua_State*, int, int);
int l_reshape(lua_State*);
int l_transpose(lua_State*);
int l_flatten(lua_State*);

#endif /* NUMLUA_MULTIARRAY_ARRAYDIMS_H_ */