

local ffi = require "ffi"

local ln=require"luandarray"


print(debug.setlocal)

-- retorna a metatable da ndarray
local function ndarray_meta()
    local _,is=debug.getupvalue(ln.type,1)
    local _, ndarray= debug.getupvalue(is,4)
    return ndarray
end


local arr=ln.array({1,2,3,4,5})
print(arr)
-- print(arr.ndarray.dtype.castfunc)

-- print(arr[2])
