#include "converts.h"
#include "arrayindex.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// void c_ndarraytostring(char *res, Ndarray *arr, size_t indent_level,
//                        const char *prefix, const char *sufix){
//     if(arr->nd == 0){
//         sprintf(res, "%g", *arr->data);
//     } else {
//         size_t i, j;
//         Ndarray *aux;
//         strcat(res, prefix);
//         for(i = 0; i < arr->dimensions[0]; i++){
//             aux = c_arrayindex(arr, i);
//             if(aux->nd == 0){
//                 char sprintf_res[20];
//                 snprintf(sprintf_res, 20, "%g\0", *aux->data);
//                 strcat(res, sprintf_res);

//                 if(i < arr->dimensions[0]-1)
//                     strcat(res, "\t");
//             } else {
//                 if(i > 0){
//                     for(j = 0; j < indent_level; j++){
//                         strcat(res, " ");
//                     }
//                 }

//                 c_ndarraytostring(res, aux, indent_level+1, prefix, sufix);

//                 if(i < arr->dimensions[0]-1){
//                     for(j = 0; j < aux->nd; j++){
//                         strcat(res, "\n");
//                     }
//                 }
//             }
//         }
//         strcat(res, sufix);
//     }
// }

// int l_ndarraytostring(lua_State *L){
//     Ndarray *arr = (Ndarray*)luaL_checkudata(L, 1, "ndarray");

//     // char res[1001];
//     char *res = malloc(sizeof(char) * (arr->size * 10) + 1);
//     printf("%u\n",sizeof(char) * arr->size + 1);
//     c_ndarraytostring(res, arr, 1, "{", "}");
//     lua_pushstring(L, res);

//     return 1;
// }

#include "converts.h"
#include "arrayindex.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lua.h>
#include <lauxlib.h>
#include <stdarg.h>

#include <stdarg.h>

// transform a Ndarray in string
void c_ndarraytostring(luaL_Buffer *b, Ndarray *arr, size_t indent_level,
                       const char *prefix, const char *sufix) {
    if (arr->nd == 0) {
        char res[15];
        snprintf(res, 15, "%g", *arr->data);
        luaL_addstring(b, res);
    } else {
        size_t i, j;
        Ndarray *aux; // auxiliar var to index

        luaL_addstring(b, prefix);

        // make a for loop in the first axis
        for(i = 0; i < arr->dimensions[0]; i++) {
            aux = c_arrayindex(arr, i);

            if (aux->nd == 0) {
                char res[15];
                snprintf(res, 15, "%g", *aux->data);
                luaL_addstring(b, res);

                if (i < arr->dimensions[0] - 1) {
                    luaL_addstring(b, "\t");
                }
            } else {
                if (i > 0) {
                    for (j = 0; j < indent_level; j++) {
                        luaL_addstring(b, " ");
                    }
                }

                c_ndarraytostring(b, aux, indent_level + 1, prefix, sufix);

                if (i < arr->dimensions[0] - 1) {
                    for (j = 0; j < aux->nd; j++) {
                        luaL_addstring(b, "\n");
                    }
                }
            }
        }

        luaL_addstring(b, sufix);
    }
}


// does the same thing as `c_ndarraytostring`, however, in a way that the Lua interpreter understands (using the API)
int l_ndarraytostring(lua_State *L) {
    Ndarray *arr = (Ndarray *)luaL_checkudata(L, 1, "ndarray");

    luaL_Buffer buffer;
    luaL_buffinit(L, &buffer);

    c_ndarraytostring(&buffer, arr, 1, "{", "}");

    luaL_pushresult(&buffer);

    return 1;
}

