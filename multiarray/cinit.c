#include <string.h>
#include <lua.h>
#include <lauxlib.h>

#include "arrayobj.h"
#include "arrayprint.h"
#include "arrayindex.h"
#include "arraydims.h"
#include "converts.h"
#include "arrayinis.h"
#include "arraymath.h"
#include "nlaux.h"
#include "lnconf.h"

static const luaL_Reg funs[] = {
    {"newNdarray", l_newNdarray},
    {"arrayprint", l_arrayprint},
    {"select", l_arrayindex},
    {"reshape", l_reshape},
    {"transpose", l_transpose},
    {"flatten", l_flatten},
    {"tostring", l_ndarraytostring},

    {"full", l_full},
    {"zeros", l_zeros},
    {"ones", l_ones},

    {"NdarrayDelete", l_NdarrayFree},

    // math functions
    {"add", l_math_add},
    {"sub", l_math_sub},
    {"mul", l_math_mul},
    {"div", l_math_sub},
    {"pow", l_math_pow},
    {"acos", l_math_acos},
    {"asin", l_math_asin},
    {"atan", l_math_atan},
    {"ceil", l_math_ceil},
    {"cos", l_math_cos},
    {"cosh", l_math_cosh},
    {"exp", l_math_exp},
    {"floor", l_math_floor},
    {"log10", l_math_log10},
    {"log", l_math_log},
    {"sin", l_math_sin},
    {"sinh", l_math_sinh},
    {"sqrt", l_math_sqrt},
    {"tan", l_math_tan},
    {"tanh", l_math_tanh},

    {"IndexStartZero", l_IndexStartZero},
    {"GetIndexStart", l_GetIndexStart},

    {NULL, NULL}
};

static const luaL_Reg methods[] = {
    // metamethods
    {"__index", l_index},
    {"__tostring", l_ndarraytostring},
    {"__gc", l_NdarrayFree},
    {"__len", l_len},
    {"__add", l_math_add},
    {"__sub", l_math_sub},
    {"__mul", l_math_mul},
    {"__div", l_math_div},
    {"__pow", l_math_pow},

    // methods
    {"select", l_arrayindex},
    {"print", l_arrayprint},
    {"item", l_item},
    {"reshape", l_reshape},

    {NULL, NULL}
};

int luaopen_cinit(lua_State *L){
    // Index start in 1 by default.
    lua_pushboolean(L, 0);
    lua_setglobal(L, "__INDEX_START_IN_ZERO");

    luaL_newmetatable(L, NDARRAY_USERDATA_NAME);
    luaLN_setfuncs(L, methods, -1);

    luaLN_newlib(L, funs);
    lua_pushstring(L, "__VERSION");
    lua_pushstring(L, LN_VERSION);
    lua_settable(L, -3);

    return 1;
}
