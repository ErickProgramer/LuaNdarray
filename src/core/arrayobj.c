#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <lua.h>
#include <lauxlib.h>

#include "arraytypes.h"
#include "error.h"
#include "arrayobj.h"
#include "vector.h"
#include "arrayindex.h"
#include "buffer.h"

void LNArray_Init(Ndarray *arr){
    arr->data=NULL;
    arr->dimensions=NULL;
    arr->strides=NULL;
    arr->dtype=NULL;

    arr->nd=0;
    arr->size=0;
}

void LNArray_Debug(const Ndarray *arr){
    size_t i;

    printf("*********************\n");
    printf("dimensions={");
    for(i=0;i<arr->nd;i++){
        printf("%zu\t",arr->dimensions[i]);
    }
    printf("}\n");
    printf("strides={");
    for(i=0;i<arr->nd;i++){
        printf("%lld\t",arr->strides[i]);
    }
    printf("}\n");
    printf("size=%zu\n",arr->size);
    printf("dtype=%s\n",LNArray_TYPE(arr)->name);

    LNBuffer repr;
    LNBuff_Init(&repr);
    for(i=0;i<arr->size;i++){
        LNBuff_addtype(&repr,arr->data+(i*arr->dtype->alignment),arr->dtype);
        LNBuff_addchar(&repr,',');
    }
    printf("arr->data={%s}\n",LNBuff_ADDR(&repr));

    printf("*********************\n");
}

Ndarray *LNArray_New(void *data, size_t *dims, long long *strides, size_t nd, const LNTypeDescr *dtype){
    Ndarray *arr = LNArray_Alloc();
    if(!arr)
        return NULL;

    arr->strides = LNMem_alloc(sizeof(long long)*nd);
    if(!arr->strides)
        return NULL;

    arr->dtype = dtype;

    arr->data = data;

    arr->dimensions = dims;
    arr->nd = nd;

    size_t i;
    arr->size = 1;
    if(!strides){
        arr->strides=LNMem_alloc(sizeof(long long)*nd);
        if(!arr->strides)
            return NULL;
        for(i = arr->nd; i > 0; i--){
            arr->strides[i-1] = arr->size*dtype->alignment;
            arr->size *= arr->dimensions[i-1];
        }
    }else{
        for(i = arr->nd; i > 0; i--){
            arr->strides[i-1] = arr->size*dtype->alignment;
            arr->size *= arr->dimensions[i-1];
        }
    }

    return arr;
}

Ndarray *LNArray_Copy(const Ndarray *arr){
    Ndarray *copy = LNArray_Alloc();
    if(!copy){
        return NULL;
    }

    copy->dtype = arr->dtype;
    copy->nd = arr->nd;
    copy->dimensions = arr->dimensions;
    copy->size = arr->size;

    copy->strides = LNMem_alloc(sizeof(size_t)*copy->nd);
    if(!copy->strides)
        return NULL;

    copy->data = LNMem_alloc(copy->dtype->alignment * copy->size);
    if(!copy->data)
        return NULL;

    if(LNArray_IsContiguous(arr) || arr->size==1){
        memcpy(copy->data, arr->data, arr->dtype->alignment * arr->size);
    } else{
        long i;
        for(i=0;i<arr->size;i+=arr->strides[arr->nd-1]){
            memcpy(copy->data+i, arr->data+i, LNArray_ALIGNMENT(arr));
        }
    }

    return copy;
}

Ndarray *LNArray_NewScalar(const void *data, const LNTypeDescr *dtype){
    Ndarray *arr = LNArray_Alloc();
    if(!arr)
        return NULL;
    LNArray_Init(arr);

    arr->data = LNMem_alloc(dtype->alignment);
    if(!arr->data)
        return NULL;

    arr->size=1;
    arr->dtype=dtype;

    memcpy(arr->data, data, arr->dtype->alignment);
    
    return arr;
}

Ndarray *LNArray_New1D(void *data, size_t ax0, const LNTypeDescr *dtype){
    Ndarray *arr = LNArray_Alloc();
    if(!arr)
        return NULL;

    arr->dtype = dtype;
    arr->size = ax0;
    arr->nd = 1;

    arr->dimensions = LNMem_alloc(sizeof(size_t));
    if(!arr->dimensions)
        return NULL;

    arr->strides = LNMem_alloc(sizeof(long long));
    if(!arr->strides)
        return NULL;

    *arr->dimensions = ax0;
    *arr->strides = LNArray_ALIGNMENT(arr);

    arr->data = data;

    return arr;
}

Ndarray *LNArray_New2D(void *data, size_t ax0, size_t ax1, const LNTypeDescr *dtype){
    Ndarray *arr = LNArray_Alloc();
    if(!arr)
        return NULL;

    arr->dimensions = LNMem_alloc(sizeof(size_t)*2);
    if(!arr->dimensions)
        return NULL;

    arr->strides = LNMem_alloc(sizeof(long long)*2);
    if(!arr->strides)    
        return NULL;

    arr->dimensions[0] = ax0;
    arr->dimensions[1] = ax1;

    arr->strides[1] = dtype->alignment;
    arr->strides[0] = arr->dimensions[0]*dtype->alignment;

    arr->dtype = dtype;
    arr->size = ax0*ax1;
    arr->nd = 2;

    arr->data = data;

    return arr;
}

int LNArray_IsContiguous(const Ndarray *arr){
    size_t i;
    size_t size=1;
    long long curstride;
    for(i=arr->nd; i>0; i--){
        curstride=size*LNArray_ALIGNMENT(arr);
        if(arr->strides[i-1] != curstride)
            return false;
        size*=arr->dimensions[i-1];
    }
    return true;
}

void LNArray_Free(Ndarray *arr){
    if(arr){
        if(arr->data)
            free(arr->data);
        if(arr->dimensions)
            free(arr->dimensions);
        if(arr->strides)
            free(arr->strides);
        // if(arr->dtype)
        //     LNDType_Free((LNTypeDescr*)LNArray_TYPE(arr));
        free(arr);
    }
}

// #include "arrayobj.h"
// #include "nlaux.h"
// #include "arrayindex.h"
// #include "arrayprint.h"
// #include "arraymath.h"
// #include "arraydims.h"
// #include "converts.h"
// #include "vector.h"

// int ln_ndarray_new(lua_State *L){
//     if(lua_gettop(L) > 1)
//         luaLN_error(L, "wrong number of arguments to 'array'");
//     luaL_argexpected(L, lua_isnumber(L, 1) || lua_istable(L, 1), 1, "number or table");

//     Ndarray *res = luaLN_converttoNdarray(L, 1);
//     luaLN_pushndarray(L, res);

//     return 1;
// }

// int ln_typeof(lua_State *L){
//     if(lua_isnone(L, 1))
//         luaLN_error(L, "bad argument #1 to 'type' (value expected)");
    
//     if(luaLN_isndarray(L, 1))
//         lua_pushstring(L, "ndarray");
//     else
//         lua_pushstring(L, luaL_typename(L, 1));

//     return 1;
// }

// int ln_isNdarray(lua_State *L){
//     if(lua_gettop(L) > 1)
//         luaLN_error(L, "wrong number of arguments to 'isNdarray'");
    
//     lua_pushboolean(L, luaLN_isndarray(L, 1));

//     return 1;
// }

// // free a Ndarray
// int ln_NdarrayFree(lua_State *L){
//     if(lua_gettop(L) > 1)
//         luaLN_error(L, "wrong number of arguments to 'NdarrayFree'");

//     Ndarray *arr = luaLN_checkndarray(L, 1);

//     if(arr){
//         free(arr->data);
//         free(arr->dimensions);
//         free(arr);
//     }

//     return 0;
// }

// // Get the shape of the Ndarray
// int ln_getshape(lua_State *L){
//     Ndarray *arr = luaLN_checkndarray(L, 1);

//     lua_newtable(L);
//     for(size_t i = 0; i < arr->nd; i++){
//         lua_pushinteger(L, arr->dimensions[i]);
//         lua_rawseti(L, -2, i+1);
//     }

//     return 1;
// }

// // Get the size of the Ndarray
// int ln_getsize(lua_State *L){
//     Ndarray *arr = luaLN_checkndarray(L, 1);

//     lua_pushinteger(L, arr->size);

//     return 1;
// }

// // convert a Ndarray to a Lua scalar
// int ln_item(lua_State *L){
//     Ndarray *arr = luaLN_checkndarray(L, 1);

//     switch(lua_gettop(L)){
//         case 1:{
//             if(arr->size != 1)
//                 luaL_error(L, "can only convert an array of size 1 to a Lua scalar");
            
//             lua_pushnumber(L, *arr->data);
//             break;
//         }
        
//         case 2:{
//             lua_Integer pos = luaLN_adjustargpos(L, 2, arr->size, 0);

//             lua_pushnumber(L, luaLN_ndarrayitem(arr, pos));
//             break;
//         }

//         default:
//             luaLN_error(L, "wrong number of arguments to 'item'");
//             break;
//     }

//     return 1;
// }

// // __len meta method
// int ln_len(lua_State *L){
//     Ndarray *arr = luaLN_checkndarray(L, 1);
//     if(arr->nd == 0)
//         luaL_error(L, "can't get length a 0-dimensional Ndarray");

//     lua_pushinteger(L, (lua_Integer)arr->dimensions[0]);

//     return 1;
// }

// // __index meta method
// int ln_index(lua_State *L){
//     Ndarray *arr = luaLN_checkndarray(L, 1);

//     switch(lua_type(L, 2)){
//         case LUA_TNUMBER:{
//             size_t idx = luaLN_adjustargpos(L, 2, arr->dimensions[0], 0);
//             Ndarray *res = c_arrayindex(arr, idx);
//             luaLN_pushndarray(L, res);
//             return 1;
//         };
//         case LUA_TTABLE: {
//             Ndarray *res = arr;
//             bool isslice = false;
//             size_t i;
//             for(i = 1; i <= luaL_len(L, 2); i++){
//                 lua_rawgeti(L, 2, i);
//                 switch(lua_type(L, -1)){
//                     case LUA_TNUMBER:
//                         if(!isslice)
//                             res = c_arrayindex(res, luaL_checkinteger(L, -1));
//                         else{
//                             res->dimensions += 1;
//                             res->nd -= 1;
//                             res->data += res->step * (res->size / arr->dimensions[0]);
//                             res->step *= (res->size / res->dimensions[0]) * res->step;
//                         }

//                         if(!arr)
//                             luaLN_MallocError(L, sizeof(Ndarray));
//                         break;
//                     case LUA_TTABLE:{
//                         isslice = true;
//                         lua_Integer start, stop, step;

//                         switch(luaL_len(L, -1)){
//                             case 1:
//                                 lua_rawgeti(L, -1, 1);
//                                 start = luaL_checkinteger(L, -1);
//                                 stop = arr->dimensions[0];
//                                 step = 1;
//                                 lua_pop(L, 1);
//                                 break;
//                             case 2:
//                                 lua_rawgeti(L, -1, 1);
//                                 start = luaL_checkinteger(L, -1);
//                                 lua_pop(L, 1);

//                                 lua_rawgeti(L, -1, 2);
//                                 stop = luaL_checkinteger(L, -1);
//                                 lua_pop(L, 1);

//                                 step = 1;
//                                 break;
                            
//                             case 3:
//                                 lua_rawgeti(L, -1, 1);
//                                 start = luaL_checkinteger(L, -1);
//                                 lua_pop(L, 1);

//                                 lua_rawgeti(L, -1, 2);
//                                 stop = luaL_checkinteger(L, -1);
//                                 lua_pop(L, 1);

//                                 lua_rawgeti(L, -1, 3);
//                                 step = luaL_checkinteger(L, -1);
//                                 lua_pop(L, 1);
//                                 break;
//                         }

//                         start -= luaLN_getIndexStart();
//                         stop -= luaLN_getIndexStart();

//                         res = c_arrayslice(res, start, stop, step);
//                         break;
//                     }
//                 }
//                 lua_pop(L, 1);
//             }
//             luaLN_pushndarray(L, res);
//             return 1;
//         };
//         case LUA_TSTRING:{
//             const char *idx = luaL_checkstring(L, 2);
//             if(strcmp(idx, "ndim") == 0)
//                 lua_pushinteger(L, arr->nd);
//             else if(strcmp(idx, "shape") == 0)
//                 ln_getshape(L);
//             else if(strcmp(idx, "size") == 0)
//                 ln_getsize(L);
//             else if(strcmp(idx, "T") == 0)
//                 ln_transpose(L);
//             else {
//                 luaL_getmetatable(L, NDARRAY_USERDATA_NAME);
//                 lua_pushvalue(L, 2);
//                 lua_rawget(L, -2);
//             }

//             return 1;
//         }

//         default:
//             lua_pushnil(L);
//             return 1;
//     }
// }

// int ln_copy(lua_State *L){
    
//     if(lua_gettop(L) > 1)
//         luaLN_error(L, "wrong number of arguments to 'copy'");

//     Ndarray *arr = luaLN_checkndarray(L, 1);

//     Ndarray *cpy = luaLN_newNdarray(L);
//     cpy->step = arr->step;
//     cpy->nd = arr->nd;
//     cpy->size = arr->size;

//     cpy->dimensions = luaLN_malloc(L, sizeof(size_t) * cpy->nd);
//     memcpy(cpy->dimensions, arr->dimensions, sizeof(size_t) * cpy->nd);

//     cpy->data = luaLN_malloc(L, sizeof(double) * arr->size);
//     if(luaLN_iscontiguous(arr)){
//         memcpy(cpy->data, arr->data, sizeof(double) * cpy->size);
//     } else {
//         size_t i;
//         for(i = 0; i < cpy->size; i++){
//             cpy->data[i] = luaLN_ndarrayitem(arr, i);
//         }
//     }


//     return 1;
// }

// int ln_IndexStartZero(lua_State *L){
//     if(lua_gettop(L) > 1)
//         luaLN_error(L, "wrong number of arguments to 'IndexStartZero'");

//     INDEX_BASE = luaLN_checkboolean(L, 1) == 0;

//     return 0;
// }

// int ln_GetIndexStart(lua_State *L){
//     if(lua_gettop(L) > 0)
//         luaLN_error(L, "wrong number of arguments to 'GetIndexStart'");

//     lua_pushinteger(L, INDEX_BASE);
//     return 1;
// }
