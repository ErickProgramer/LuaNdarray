#include "arrayobj.h"
#include "arrayindex.h"
#include "nlaux.h"

#include <stdio.h>
#include <stdlib.h>

// prints an Ndarray
void c_arrayprint(Ndarray *arr, size_t indent_level){
    if(arr->nd == 0){
        printf("%g\n", *arr->data);
    } else {
        size_t i, j;
        Ndarray *aux;

        printf("{");
        for(i = 0; i < arr->dimensions[0]; i++){
            aux = c_arrayindex(arr, i);
            if(aux->nd == 0){
                printf("%g\t", *aux->data);
            } else {
                if(i > 0)
                    nL_repprint(" ", indent_level);
                    
                c_arrayprint(aux, indent_level + 1);

                if(i < arr->dimensions[0]-1)
                    nL_repprint("\n", arr->nd-1);
            }
        }
        printf("}");
    }
}

// works the same as c_arrayprint, but in such a way that the Lua interpreter recognizes
int l_arrayprint(lua_State *L){
    Ndarray *arr = (Ndarray*)luaL_checkudata(L, 1, "ndarray");
    c_arrayprint(arr, 1);
    return 0;
}
