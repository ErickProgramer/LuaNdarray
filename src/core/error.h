#ifndef LUANDARRAY_SRC_CORE_ERROR_H_
#define LUANDARRAY_SRC_CORE_ERROR_H_

#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>

#include<lua.h>
#include<lauxlib.h>

typedef struct LNError_t{
    char *errmsg;
    char err;
    #if defined(LUA)
    lua_State *L;
    #endif
} LNError_t;

LNError_t LNExc_Error;

#define LNError_Check()\
    do {\
        if(LNError_Ocurred()){\
            LNError_Show();\
            exit(1);\
        }\
    } while (0)\

#define LNError_CheckNULL()\
    do{\
        if(LNError_Ocurred()){\
            return NULL;\
        }\
    } while(0)

void LNError_Init();


void LNError_setString(const char *err);
void LNError_setFString(const char *fmt, ...);
void LNError_setState(lua_State *L);

#if defined(LUA)
void LNError_Run();
#endif

char LNError_Ocurred();
char *LNError_Get();

void LNError_Show();
void LNError_Remove();

#define LNError_Raise()\
    do{\
        fprintf(stderr, "LuaNdarray error ocurred:\n\t%s:%d: %s", __FILE__, __LINE__, LNError_Get());\
        exit(1);\
    }while(0)

#endif