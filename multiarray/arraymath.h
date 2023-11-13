#ifndef LUANDARRAY_MULTIARRAY_ARRAYMATH_H_
#define LUANDARRAY_MULTIARRAY_ARRAYMATH_H_

#include "arrayobj.h"

#include <lua.h>
#include <lauxlib.h>

typedef double (*OperationFunction)(double x, double y);
typedef double (*MathFunction)(double x);

int c_broadcast_to(lua_State *L, Ndarray *result, const Ndarray *arr, const size_t *new_dims, size_t new_ndim);

int l_math_add(lua_State *L);
int l_math_sub(lua_State *L);
int l_math_mul(lua_State *L);
int l_math_div(lua_State *L);
int l_math_pow(lua_State *L);

int l_math_sin(lua_State *L);
int l_math_cos(lua_State *L);
int l_math_tan(lua_State *L);
int l_math_sinh(lua_State *L);
int l_math_cosh(lua_State *L);
int l_math_tanh(lua_State *L);
int l_math_asin(lua_State *L);
int l_math_acos(lua_State *L);
int l_math_atan(lua_State *L);
int l_math_exp(lua_State *L);
int l_math_log(lua_State *L);
int l_math_log10(lua_State *L);
int l_math_sqrt(lua_State *L);
int l_math_ceil(lua_State *L);
int l_math_floor(lua_State *L);
int l_math_fabs(lua_State *L);


#endif /* LUANDARRAY_NULTIARRAT_ARRAYMATH_H_ */