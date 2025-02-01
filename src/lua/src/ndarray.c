#include<stdio.h>
#include<stdlib.h>

#include<lua.h>
#include<lauxlib.h>

#include "luand.h"
#include "conf.h"
#include "ndarray.h"

static const luaL_Reg metatable[] = {
    {"__tostring", ln_array__tostring},
    {"__gc", ln_array__gc},
    //{"__add", ln_array__add},
    {NULL, NULL}
};

void ln_array_init(lua_State *L){
    luaL_newmetatable(L, LN_NDARRAY_MT);;
    luaND_setfuncs(L, metatable, 0);
    lua_pop(L, 1);
}


/* auxiliar functions */
 size_t *ln_table2dims(lua_State *L, int idx, size_t l){
    size_t *vec = (size_t*)luaND_alloc(L, sizeof(size_t) * l);
    lua_pushvalue(L, idx);

    size_t i;
    for(i = 0; i < l; i++){
        lua_pushinteger(L, (lua_Integer)i + 1);
        lua_gettable(L, -2);

        lua_Integer num = lua_tonumber(L, -1);
        if(num < 0){
            luaL_error(L, "negatives index are not allowed");
            return NULL;
        }

        vec[i] = (size_t)num;
        lua_pop(L, 1);
    }

    return vec;
}

#define ln_tonumber(L, idx) (lua_isboolean((L), (idx)) ? lua_toboolean((L), (idx)) : lua_tonumber((L), (idx)))

#define CASE(id, t)\
    case id:\
        for(i = 0; i < n; i++){\
            lua_rawgeti(L, idx, i+1);\
            ((t*)vec)[i] = (t)ln_tonumber(L, -1);\
            lua_pop(L, 1);\
        }\
    break\

// transforms a lua table into a C array based on the `dtype`
 void *ln_table2vector(lua_State *L, int idx, const LNTypeDescr *dtype){
    size_t i, n;
    n = luaND_len(L, idx);

    void *vec = luaND_alloc(L, dtype->alignment * n);
    switch(dtype->id){
        case LN_UINT8:
        CASE(LN_INT8, int8_t);

        case LN_UINT16:
        CASE(LN_INT16, int16_t);

        case LN_UINT32:
        CASE(LN_INT32, int32_t);
        
        case LN_UINT64:
        CASE(LN_INT64, int64_t);

        CASE(LN_FLOAT32, float);
        CASE(LN_FLOAT64, double);
    }

    return vec;
}
/********************************* */

 int ln_array_zeros(lua_State *L){
    size_t ndim = luaND_len(L, 1);
    size_t *dims = ln_table2dims(L, 1, ndim);
    const LNTypeDescr *dtype = luaND_optdtype(L, 2, LNDType_GetFromID(LN_FLOAT64));
    
    Ndarray *arr = LNArray_Zeros(dims, ndim, dtype);
    if(LNError_Ocurred()){
        luaL_error(L, LNError_Get());
        return 0;
    }

    luaND_pushndarray(L, arr);
    return 1;
}

int ln_array_ones(lua_State *L){
    size_t ndim = luaND_len(L, 1);
    size_t *dims = ln_table2dims(L, 1, ndim);

    const LNTypeDescr *dtype = luaND_optdtype(L, 2, LNDType_GetFromID(LN_FLOAT64));
    
    Ndarray *arr = LNArray_Ones(dims, ndim, dtype);
    if(LNError_Ocurred()){
        luaL_error(L, LNError_Get());
        return 0;
    }

    luaND_pushndarray(L, arr);

    return 1;
}

 int ln_array_empty(lua_State *L){
    size_t ndim = luaND_len(L, 1);
    size_t *dims = ln_table2dims(L, 1, ndim);
    const LNTypeDescr *dtype = luaND_optdtype(L, 2, LNDType_GetFromID(LN_FLOAT64));
    
    Ndarray *arr = LNArray_Empty(dims, ndim, dtype);
    if(LNError_Ocurred()){
        luaL_error(L, LNError_Get());
        return 0;
    }

    luaND_pushndarray(L, arr);
    return 1;
}

 int ln_array_arange(lua_State *L){
    lua_Number start, stop, step;
    const LNTypeDescr *dtype;
    switch(lua_gettop(L)){
        case 2:
            start = 1;
            stop = luaL_checknumber(L, 1);
            step = 1;
            dtype = luaND_optdtype(L, 2, LNDType_GetFromID(LN_FLOAT64));

            break;

        case 3:
            start = luaL_checknumber(L, 1);
            stop = luaL_checknumber(L, 2);
            step = 1;
            dtype = luaND_optdtype(L, 3, LNDType_GetFromID(LN_FLOAT64));
            break;

        case 4:
            start = luaL_checknumber(L, 1);
            stop = luaL_checknumber(L, 2);
            step = luaL_checknumber(L, 3);
            dtype = luaND_optdtype(L, 4, LNDType_GetFromID(LN_FLOAT64));
            break;

        default:
            luaL_error(L, "wrong number of arguments to 'arange'");
            break;
    }
    Ndarray *res = LNArray_Range(start, stop, step, dtype);
    luaND_pushndarray(L, res);

    return 1;
}

static void collectInfo(lua_State *L, Ndarray *out){
    while(1){
        out->nd++;
        out->dimensions = luaND_realloc(L, out->dimensions, sizeof(size_t)*out->nd);
        out->dimensions[out->nd-1] = luaND_len(L, -1);

        lua_rawgeti(L, -1, 1);
        if(!lua_istable(L, -1)){
            break;
        }
    }
    lua_pop(L, out->nd-1);

    out->strides = luaND_alloc(L, sizeof(long long) * out->nd);

    size_t i;
    out->size = 1;
    for(i=0; i < out->nd; i++){
        out->strides[out->nd - i - 1] = sizeof(LNArray_ALIGNMENT(out))*out->size;
        out->size *= out->dimensions[i];
    }

    out->data = luaND_alloc(L, LNArray_ALIGNMENT(out)*out->size);

    size_t *INDEX = luaND_alloc(L, sizeof(size_t)*out->nd);
    for(i=0; i<out->nd; i++){
        INDEX[i] = 0;
    }
}

static int ln_array_int32build(lua_State *L){
    Ndarray *arr = LNArray_Alloc();
    LNArray_Init(arr);
    arr->dtype = LNDType_GetFromID(LN_INT32);
    collectInfo(L, arr);

    size_t i, p = 0;
    size_t *INDEX = luaND_alloc(L, sizeof(size_t)*arr->nd);
    while(1){
        for(i=0; i < arr->nd; i++){
            lua_rawgeti(L, -1, INDEX[i]+1);
        }
        p++;

        arr->data[p] = luaL_checknumber(L, -1);

        if(p == arr->size){
            break;
        }

        INDEX[arr->nd-1]++;
    }

    luaND_pushndarray(L, arr);
    return 1;
}

// function ln.ndarray(data, shape, strides, dtype)

static const LNTypeDescr *ln_aux_getdtype(lua_State *L, int obj){
    int tp = lua_type(L, obj);

    if(tp == LUA_TNUMBER){
        luaND_pushdtype(L, LN_INT64);
    }
}

int ln_array_new(lua_State *L){
    int top = lua_gettop(L);

    if(top == 2){ /* flags? */
        lua_getfield(L, 2, "shape");
        lua_getfield(L, 2, "strides");
        lua_getfield(L, 2, "dtype");
        lua_remove(L, 2);
    } else if(top != 4){
        luaL_error(L, "wrong number of arguments to 'ndarray'");
        return 0;
    }

    size_t *shape            = NULL;
    size_t nd                = 0;
    size_t size              = 1;
    const LNTypeDescr *dtype = luaND_checkdtype(L, 4);

    switch(luaND_type(L, 2)){
        case LUA_TTABLE:
            nd = luaND_len(L, 2);
            shape = ln_table2dims(L, 2, nd);

            /* is not needed to calculate the size if strides are nil */
            if(!lua_isnil(L, 3)){
                size_t i;
                for(i=0; i < nd; i++){
                    size *= shape[i];
                }
            }
            break;
    }

    long long *strides = luaND_alloc(L, sizeof(long long)*nd);
    if(lua_isnil(L, 3)){ /* strides */
        size_t i;
        size = 1;
        for(i = nd; i > 0; i--){
            strides[i-1] = size*dtype->alignment;
            size *= shape[i-1];
        }
    } else {
        size_t i;
        for(i=0; i < nd; i++){
            lua_rawgeti(L, 3, i+1);
            if(!lua_isnumber(L, -1)){
                free(strides);
                free(shape);
                luaL_error(L, "%s cannot be interpreted as an integer", luaL_typename(L, -1));
                return 0;
            }
    
            strides[i] = lua_tonumber(L, -1);

            lua_pop(L, 1);
        }
    }


    size_t i;
    Ndarray *res = (Ndarray*)luaND_alloc(L, sizeof(Ndarray));

    res->data        = ln_table2vector(L, 1, dtype);
    res->size        = size;
    res->nd          = nd;
    res->dimensions  = shape;
    res->strides     = strides;
    res->dtype       = dtype;

    luaND_pushndarray(L, res);

    return 1;
}

//int ln_array(lua_State *L){
//    const LNTypeDescr *dtype = luaND_checkdtype(L, 2);
//
//    size_t ndim = 0, i;
//    size_t *shape = malloc(0);
//
//    lua_pushvalue(L, 1);
//    while (lua_type(L, -1) == LUA_TTABLE){
//        ndim += 1;
//
//
//        if(lua_type(L, -1) == LUA_TTABLE){
//            size_t len = lua_objlen(L, -1);
//            lua_rawgeti(L, 1);
//            size_t exp_len = lua_objlen(L, 1);
//
//            for(i=2; i <= len; i++){
//                lua_rawgeti(L, i);
//                if(mustbe_table && lua_type(L, -1) != LUA_TTABLE){
//                    
//                }
//
//                size_t sub_len = lua_objlen(L, -1);
//                if(sub_len != exp_len){
//
//                }
//                lua_pop(L, 1);
//            }
//        }
//
//        if(!realloc(shape, sizeof(size_t)*ndim)){
//            luaL_error(L, "was not possible to allocate the shape vector")
//            return 0;
//        }
//
//        shape[ndim-1] = len;
//    }
//    lua_pop(L, ndim);
//
//}

// __tostring meta method
int ln_array__tostring(lua_State *L){
    Ndarray *arr = luaND_checkndarray(L, 1);

    char *repr = LNArray_toString(arr, "array(", ")");
    if(LNError_Ocurred()){
        luaL_error(L, LNError_Get());
        return 0;
    }

    lua_pushstring(L, repr);
    return 1;
}

int ln_array__gc(lua_State *L){
    Ndarray *arr = luaND_checkndarray(L, 1);
    LNArray_Free(arr);
    return 0;
}

