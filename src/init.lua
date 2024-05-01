if jit then
    return require "luandarray.luajit.init"
else
    error "luandarray does not has support for Lua."
end