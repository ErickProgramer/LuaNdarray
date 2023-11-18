---@diagnostic disable: lowercase-global
package = "LuaNdarray"
version = "1.0.0-1"

source = {
    url = "git+https://github.com/ErickProgramer/LuaNdarray.git"
}

description = {
    summary = "Ndarray manipulation quickly and optimally",
    detailed = [[
        LuaNdarray is a library for manipulating Ndarray
        offering several valuable functions for manipulating Ndarray
        for now we do not have support for dtypes but over time this will be added!
    ]],

    homepage = "https://github.com/ErickProgramer/LuaNdarray",

    license = "BSD 3-clause"
}

dependencies = {
    "lua >= 5.1"
}

build = {
    type = "builtin",
    modules = {
        ["luandarray.core"] = {
            sources = {"multiarray/arraydims.c",
                       "multiarray/arrayindex.c",
                       "multiarray/arrayinis.c",
                       "multiarray/arrayobj.c",
                       "multiarray/arrayprint.c",
                       "multiarray/cinit.c",
                       "multiarray/converts.c",
                       "multiarray/nlaux.c",
                       "multiarray/arraymath.c"
                      }
        },

        ["luandarray.init"] = "multiarray/init.lua"
    }
}
