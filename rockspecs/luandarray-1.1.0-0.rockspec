---@diagnostic disable: lowercase-global
package = "LuaNdarray"
version = "1.1.0-0"

source = {
    url = "git+https://github.com/ErickProgramer/LuaNdarray.git",
    tag = "v1.1.0"
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
    type = "make",

    variables = {
        LIB_EXTENSION   = "$(LIB_EXTENSION)",
    },
    
    build_variables = {
        CFLAGS     = "$(CFLAGS)",
        LIBFLAG    = "$(LIBFLAG)",
        LUA_LIBDIR = "$(LUA_LIBDIR)",
        LUA_BINDIR = "$(LUA_BINDIR)",
        LUA_INCDIR = "$(LUA_INCDIR)",
        LUA        = "$(LUA)",
        LDFLAGS    = "$(LIBFLAG)",
    },

    install_variables = {
        ENV_INST_PREFIX  = "$(PREFIX)",
        ENV_INST_BINDIR  = "$(BINDIR)",
        ENV_INST_LIBDIR  = "$(LIBDIR)",
        ENV_INST_LUADIR  = "$(LUADIR)",
        ENV_INST_CONFDIR = "$(CONFDIR)",
    },
}