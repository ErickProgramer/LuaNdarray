#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdarg.h>

#include "error.h"

static LNError_t LNExc_Error;

void LNError_Init(){
    #ifdef LUA
    LNExc_Error.L = NULL;
    #endif
    LNExc_Error.errmsg = NULL;
    LNExc_Error.err = 0;
}

#if defined(LUA)
void LNError_setState(lua_State *L){
    LNExc_Error.L = L;
}
#endif

void LNError_setString(const char *errmsg){
    LNError_Remove();

    size_t len=strlen(errmsg);
    LNExc_Error.errmsg = malloc(len+1);
    strncpy(LNExc_Error.errmsg, errmsg, len);
    LNExc_Error.errmsg[len]='\0';
    LNExc_Error.err = 1;
}

void LNError_setFString(const char *fmt, ...){
    va_list args;
    va_start(args, fmt);
    int n = vsnprintf(NULL, 0, fmt, args);
    if(n<0)n=0;
    va_end(args);

    va_start(args, fmt);
    char *err = malloc(n+1);
    if(!err)
        err="Fatal Error: error on allocate the message error";
    vsnprintf(err, n+1, fmt, args);
    va_end(args);

    LNError_Remove();
    LNExc_Error.errmsg = err;
    LNExc_Error.err = 1;
}

char LNError_Ocurred(){
    return LNExc_Error.err;
}

char *LNError_Get(){
    return LNExc_Error.errmsg;
}

void LNError_Show(){
    printf("%s\n", LNExc_Error.errmsg);
}

#if defined(LUA)
void LNError_Run(){
    luaL_error(LNExc_Error.L, LNError_Get())
}
#endif

void LNError_Run(){
    #ifdef LUA
    luaL_error(LNExc_Error.L, LNExc_Error.errmsg);
    #endif
}

void LNError_Remove(){
    if(LNExc_Error.errmsg)
        free((char*)LNExc_Error.errmsg);
    LNExc_Error.err = 0;
}

