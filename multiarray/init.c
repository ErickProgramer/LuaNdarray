#include <lua.h>
#include <lauxlib.h>

#include "arrayobj.h"
#include "arrayprint.h"
#include "arrayindex.h"


static const luaL_Reg funs[] = {
    {"newNdarray", l_newNdarray},
    {"arrayprint", l_arrayprint},
    {"arrayindex", l_arrayindex},
    {NULL, NULL}
};

int luaopen_init(lua_State *L){
    luaL_newmetatable(L, "ndarray");
    lua_pushstring(L, "__index");
    lua_pushvalue(L, -2);
    lua_settable(L, -3);

    luaL_newlib(L, funs);
    return 1;
}
