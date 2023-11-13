#include "arraymath.h"
#include "arrayobj.h"
#include "arrayindex.h"
#include "arraydims.h"
#include "nlaux.h"
#include "arrayprint.h"

#include <math.h>
#include <string.h>
#include <lua.h>
#include <lauxlib.h>

/*!
 * Performs broadcasting in the array `arr` placing the result in `result`
 * @param L the Lua Stack. If you do not want the function to use the LUA API functions, pass `NULL`.
 * @param result where the function will place the broadcasting result
 * @param arr Ndarray to be transmitted
 * @param new_dims The dimension to match
 * @param new_ndim The length of `new_dims`
 * @return returns an integer representing whether the function was successful or not, returns `1` if successful `0` if not.
 */
int c_broadcast_to(lua_State *L, Ndarray *result, const Ndarray *arr, const size_t *new_dims, size_t new_ndim){
    // Useful variables
    size_t i, j, size, start, stop, expanded_size, diff;

    // calculate the size of new_dims
    size_t new_size = 1;
    for(i = 0; i < new_ndim; i++)
        new_size *= new_dims[i];

    result->nd = new_ndim;
    result->size = arr->size;

    if(L){ // if L is not NULL, functions from `nlaux` and the LUA API will be used
        result->dimensions = luaLN_malloc(L, sizeof(size_t) * result->nd);
        result->data = luaLN_malloc(L, sizeof(double) * new_size);
        result->size = arr->size;
    } else {  // Otherwise, no API or nlaux functions will be used.
        result->dimensions = malloc(sizeof(size_t) * result->nd);
        if(!result->dimensions) return 0;
    
        result->data = malloc(sizeof(double) * new_size);
        if(!result->data) return 0;
    }

    // Expands dimensions with `1` at the beginning to match dimensions
    diff = new_ndim - arr->nd;
    for(i = 0; i < diff; i++)
        result->dimensions[i] = 1;

    // Initializing the fields
    memcpy(result->data, arr->data, sizeof(double) * arr->size);
    memcpy(result->dimensions + diff, arr->dimensions, sizeof(size_t) * arr->nd);

    // Checks whether the broadcast is valid
    for(i = 0; i < result->nd; i++){
        if(!((result->dimensions[i] == new_dims[i]) || (result->dimensions[i] == 1 || new_dims[i] == 1))){
            if(L) // If L is not `NULL`, luaLN_error will be used
                luaLN_error(L, "operands could not be broadcast, incompatible shape");
            else // if the LUA API is not used, it only returns an error code.
                return 0;
        }
    }

    // Check if the dimensions are the same
    int isequal = 1;
    for(i = 0; i < result->nd; i++){
        if(result->dimensions[i] != new_dims[i]){
            isequal = 0;
            break;
        }
    }
    // If the dimensions are the same, broadcasting will not be necessary.
    if(isequal)
        return 1;

    // Expanded version of a given axis
    double *expanded = malloc(sizeof(double) * new_size);
    if((L != NULL) && (!expanded)) luaLN_MallocError(L, sizeof(double) * new_size);
    if(!expanded) return 0;

    i = new_ndim;
    while(i > 0){
        i--;
        diff = new_dims[i] - result->dimensions[i] + 1;

        size = 1;
        for(j = i; j < result->nd; j++) size *= result->dimensions[j];

        expanded_size = 0;
        for(start = 0; start < result->size; start += size){
            stop = start + size;
            for(j = 0; j < diff; j++){
                for(size_t z = start; z < stop; z++){
                    expanded[expanded_size] = result->data[z];
                    expanded_size++;
                }
            }
        }
        result->size = expanded_size;
        memcpy(result->data, expanded, sizeof(double) * expanded_size);
        result->dimensions[i] = new_dims[i];

    }

    free(expanded);

    return 1;
}


int l_broadcast_to(lua_State *L){
    Ndarray *arr = luaLN_checkndarray(L, 1);
    
    size_t new_ndim = luaL_len(L, 2);
    size_t *new_dims = malloc(sizeof(size_t) * new_ndim);

    for(size_t i = 0; i < new_ndim; i++){
        lua_rawgeti(L, -1, i+1);
        new_dims[i] = lua_tonumber(L, -1);
        lua_pop(L, 1);
    }

    Ndarray *res = luaLN_newNdarray(L);
    c_broadcast_to(L, res, arr, new_dims, new_ndim);
    luaLN_pushndarray(L, res);

    return 1;
}

static int new_mathoperation(lua_State *L, OperationFunction f){
    Ndarray *arr1 = luaLN_checkndarray(L, 1);
    Ndarray *arr2 = luaLN_checkndarray(L, 2);

    Ndarray *min_arr = arr1;
    Ndarray *max_arr = arr2;
    if(arr2->size < arr1->size){
        min_arr = arr2;
        max_arr = arr1;
    }

    // Version expanded of the min array
    Ndarray *min_expanded = luaLN_newNdarray(L);
    c_broadcast_to(L, min_expanded, min_arr, max_arr->dimensions, max_arr->nd);

    Ndarray *res = luaLN_newNdarray(L);
    res->size = max_arr->size;
    res->nd = max_arr->nd;

    res->dimensions = luaLN_malloc(L, sizeof(size_t) * res->nd);
    memcpy(res->dimensions, max_arr->dimensions, sizeof(size_t) * max_arr->nd);

    res->data = luaLN_malloc(L, sizeof(double) * res->size);

    size_t i;
    for(i = 0; i < max_arr->size; i++)
        res->data[i] = min_expanded->data[i] + max_arr->data[i];

    return 0;
}

static int new_mathfunction(lua_State *L, MathFunction f){
    if(lua_type(L, 1) == LUA_TNUMBER){
        lua_pushnumber(L, f(luaL_checknumber(L, 1)));
        return 1;
    }
    Ndarray *arr = luaL_checkudata(L, 1, "ndarray");
    
    Ndarray *res = lua_newuserdata(L, sizeof(Ndarray));

    luaL_getmetatable(L, "ndarray");
    lua_setmetatable(L, -2);

    res->size = arr->size;
    res->nd = arr->nd;
    res->dimensions = luaLN_malloc(L, sizeof(size_t) * res->nd);
    memcpy(res->dimensions, arr->dimensions, sizeof(size_t) * arr->nd);
    
    res->data = luaLN_malloc(L, sizeof(double) * arr->size);

    size_t i;
    for(i = 0; i < res->size; i++)
        res->data[i] = f(arr->data[i]);

    return 1;
}

static double __add(double x, double y){return x+y;}
static double __sub(double x, double y){return x-y;}
static double __mul(double x, double y){return x*y;}
static double __div(double x, double y){return x/y;}

int l_math_add(lua_State *L){
    new_mathoperation(L, __add);
    return 1;
}

int l_math_sub(lua_State *L){
    new_mathoperation(L, __sub);
    return 1;
}

int l_math_div(lua_State *L){
    new_mathoperation(L, __div);
    return 1;
}

int l_math_mul(lua_State *L){
    new_mathoperation(L, __mul);
    return 1;
}

int l_math_pow(lua_State *L){
    new_mathoperation(L, pow);
    return 1;
}

int l_math_sin(lua_State *L){ new_mathfunction(L, sin); return 1; }
int l_math_cos(lua_State *L){ new_mathfunction(L, cos); return 1; }
int l_math_tan(lua_State *L){ new_mathfunction(L, tan); return 1; }
int l_math_sinh(lua_State *L){ new_mathfunction(L, sinh); return 1; }
int l_math_cosh(lua_State *L){ new_mathfunction(L, cosh); return 1; }
int l_math_tanh(lua_State *L){ new_mathfunction(L, tanh); return 1; }
int l_math_asin(lua_State *L){ new_mathfunction(L, asin); return 1; }
int l_math_acos(lua_State *L){ new_mathfunction(L, acos); return 1; }
int l_math_atan(lua_State *L){ new_mathfunction(L, atan); return 1; }
int l_math_exp(lua_State *L){ new_mathfunction(L, exp); return 1; }
int l_math_log(lua_State *L){ new_mathfunction(L, log); return 1; }
int l_math_log10(lua_State *L){ new_mathfunction(L, log10); return 1; }
int l_math_sqrt(lua_State *L){ new_mathfunction(L, sqrt); return 1; }
int l_math_ceil(lua_State *L){ new_mathfunction(L, ceil); return 1; }
int l_math_floor(lua_State *L){ new_mathfunction(L, floor); return 1; }
int l_math_fabs(lua_State *L){ new_mathfunction(L, fabs); return 1; }