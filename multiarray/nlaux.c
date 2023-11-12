#include "nlaux.h"
#include "arrayobj.h"

#include <lua.h>
#include <lauxlib.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/*
 * repeat `printf`
 * NOTE: This function will be removed soon
 */
void nL_repprint(const char *txt, size_t q){
    for(size_t i = 0; i < q; i++)
        printf(txt);
}

/*
 * returns the value in `data` with the largest size as a string using the format `%g`. (currently not used)
 * NOTE: This function will be removed soon
*/
size_t nL_getmaxalign(double *data, size_t data_len){
    char *str = NULL;
    sprintf(str, "%g", *data);

    size_t len, max = strlen(str);
    for(size_t i = 1; i < data_len; i++){
        sprintf(str, "%g", data[i]);
        len = strlen(str);
        if(len > max) max = len;
    }

    return max;
}

/*!
 * allocates memory. Unlike traditional `malloc`, in this you don't need to worry about checking whether the allocation was successful or not, as this function triggers an error in `L` automatically.
 * @param L Stack
 * @param bytes bytes to allocate
 * @param detailed put `1` if you want a more detailed error message, another number if not.
 * @return the allocated memory
 */
void *luaLN_mallocd(lua_State *L, size_t bytes, int detailed){
    void *r = malloc(bytes);

    if(r == NULL){
        if(detailed)
            luaLN_error(L, "MemoryError: error allocating %zu bytes.", bytes);
        else
            luaLN_error(L, "MemoryError: insufficient memory");
    }

    return r;
}

/*! Does not allocate memory, just displays the error message based on the information
 * @param L the Lua Stack
 * @param bytes The bytes you tried to allocate
 * @param detailed put `1` if you want a more detailed error message, another if not
 */
void luaLN_MallocdError(lua_State *L, size_t bytes, int detailed){
    if(detailed)
        luaL_error(L, "MemoryError: error allocating %zu bytes.", bytes);
    else
        luaL_error(L, "MemoryError: insufficient memory");
}

/*! creates an Ndarray by returning it and placing it on the Stack
 * @param L the Lua Stack
 * @param setmetatable If you want it to place the metatable in the Ndarray that is placed on the Stack. `1` if yes, another if no
 * @return the allocated Ndarray
 */
Ndarray *luaLN_newNdarraym(lua_State *L, int setmetatable){
    Ndarray *arr = (Ndarray*)luaLN_malloc(L, sizeof(Ndarray));

    lua_pushlightuserdata(L, arr);
    if(setmetatable){
        luaL_getmetatable(L, NDARRAY_USERDATA_NAME);
        lua_setmetatable(L, -2);
    }

    return arr;
}

/*! checks if the number of arguments is as expected
 * @param fname function name
 * @param limit the limit of arguments that can be passed into the function
 */
void luaLN_checkargcount(lua_State *L, const char *fname, size_t limit){
    if(lua_gettop(L) > limit)
        luaL_error(L, "wrong number of arguments to '%s'", fname);
}

/*! works like calloc but displays error automatically if allocation is not successful
 * @param L the Lua Stack
 * @param q amount you want to allocate
 * @param bytes bytes to allocate
 * @param detailed If you want a more detailed error message if an allocation error occurs
 * @return the allocated memory
 */
void *luaLN_callocd(lua_State *L, size_t q, size_t bytes, int detailed){
    void *r = calloc(q, bytes);

    if(!r){
        if(detailed)
            luaL_error(L, "MemoryError: error on allocate %zu bytes.", q * bytes);
        else
            luaL_error(L, "MemoryError: insufficient memory");
    }

    return r;
}

/*! puts an Ndarray on the stack
 * @param L the Lua Stack
 * @param arr the Ndarray to be placed in `L`
 * @param setmetatable If you want the function to automatically place methods and meta methods (`1` if yes, other if no)
 */
void luaLN_pushndarraym(lua_State *L, Ndarray *arr, int setmetatable){
    lua_pushlightuserdata(L, (void*)arr);

    if(setmetatable){
        luaL_getmetatable(L, NDARRAY_USERDATA_NAME);
        lua_setmetatable(L, -2);
    }
}

/*! checks whether an argument is of boolean type and returns it
 * @param L the Lua Stack
 * @param arg the argument where the value you want to check is located
 * @return an integer that represents a boolean. `1` = `true`, `0` = `false`
 */
int luaLN_checkboolean(lua_State *L, int arg){
    luaL_checktype(L, arg, LUA_TBOOLEAN);
    return lua_toboolean(L, arg);
}

/*! returns at which index indexing starts. `0` if it starts at 0, `1` if it starts at 1
 * @param L the Lua Stack
 */
int luaLN_getIndexStart(lua_State *L){
    lua_getglobal(L, "__INDEX_START_IN_ZERO");
    int start_in_zero = lua_toboolean(L, -1);
    lua_pop(L, 1);

    if(start_in_zero)
        return 0;
    return 1;
}

/*! adjusts the position `pos` if it is negative the function corrects it using `limit` as a base and using `axis` for the error message
 * @param L the Lua Stack
 * @param pos the position to index
 * @param limit the limit size
 * @param axis the axis that was indexed (used for the error message)
 * @return returns the already adjusted index
 */
size_t luaLN_adjustpos(lua_State *L, lua_Integer pos, size_t limit, size_t axis){
    int start = luaLN_getIndexStart(L);
    size_t pos_sizet;

    if(pos < 0){
        if((size_t)((-pos)) > limit)
            luaLN_error(L, "index %lld is out of bounds for axis %zu with size %zu (index starting at %d)", pos, axis + start, limit, start);

        pos_sizet = limit - (size_t)((-pos));
    }
    else{
        if((pos == 0 && start == 1))
            luaLN_error(L, "index %lld is out of bounds for axis %zu with size %zu (index starting at %d)", pos, axis + start, limit, start);
        
        pos_sizet = (size_t)pos - start;
    }

    if(pos_sizet >= limit)
        luaLN_error(L, "index %lld is out of bounds for axis %zu with size %zu (index starting at %d)", pos, axis + start, limit, start);

    return pos_sizet;
}

/*! places the `funcs` functions in the table at the specified position
 * @param L the Lua Stack
 * @param funcs the functions
 * @param tablepos the position of the table to place the functions
 */
void luaLN_setfuncs(lua_State *L, const luaL_Reg *funcs, int tablepos){
    if(tablepos < 0)
        tablepos -= 2;
    else
        tablepos += 2;

    size_t i;
    for(i = 0; funcs[i].func && funcs[i].name; i++){
        lua_pushstring(L, funcs[i].name);
        lua_pushcfunction(L, funcs[i].func);
        lua_settable(L, tablepos);
    }
}

/*! create a table with the past functions
 * @param L the Lua Stack
 * @param lib library functions
 */
void luaLN_newlib(lua_State *L, const luaL_Reg *lib){
    lua_newtable(L);
    luaLN_setfuncs(L, lib, -1);
}

/*! works like `luaL_error` but with the default formatting types of `printf`
 * @param L the Lua Stack
 * @param fmt the error message
 */
void luaLN_error(lua_State *L, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    int len = vsnprintf(NULL, 0, fmt, args);

    char *error = malloc(len + 1);

    va_end(args);
    va_start(args, fmt);

    vsnprintf(error, len + 1, fmt, args);

    luaL_error(L, "%s", error);

    va_end(args);
    free(error);
}
