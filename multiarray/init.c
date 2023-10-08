#include <lua.h>
#include <lauxlib.h>

#include "arrayobj.h"
#include "arrayprint.h"
#include "arrayindex.h"
#include "arraydims.h"
#include "converts.h"
#include "arrayinis.h"

static const luaL_Reg funs[] = {
    {"newNdarray", l_newNdarray},
    {"arrayprint", l_arrayprint},
    {"arrayindex", l_arrayindex},
    {"reshape", l_reshape},
    {"transpose", l_transpose},
    {"flatten", l_flatten},
    {"tostring", l_ndarraytostring},
    {"full", l_full},
    {"zeros", l_zeros},
    {"array", l_ndarray_by_table},
    {"NdarrayDelete", l_NdarrayFree},
    {NULL, NULL}
};

static const luaL_Reg methods[] = {
    // metamethods
    {"__tostring", l_ndarraytostring},
    {"__gc", l_NdarrayFree},

    // methods
    {"select", l_arrayindex},
    {"print", l_arrayprint},
    {NULL, NULL}
};

int luaopen_cinit(lua_State *L){
    luaL_newmetatable(L, "ndarray");
    lua_pushstring(L, "__index");
    lua_pushvalue(L, -2);
    lua_settable(L, -3);

    luaL_setfuncs(L, methods, 0);

    luaL_newlib(L, funs);
    return 1;
}
