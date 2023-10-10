if jit then error "LuaNdarray dont have support for LuaJit per while" end

---@diagnostic disable: missing-return

---@class ln.Ndarray
local Ndarray = {}

function Ndarray:print() end

---@return ln.Ndarray
function Ndarray:select(idx) end

---@class LuaNdarray
local luandarray = {}

---@param data table
---@param shape table
---@return ln.Ndarray
function luandarray.newNdarray(data, shape) end

---@param array ln.Ndarray
function luandarray.arrayprint(array) end

---@param array ln.Ndarray
---@param idx integer
---@return ln.Ndarray
function luandarray.arrayindex(array, idx) end

---@param array ln.Ndarray
---@param new_shape table
---@return ln.Ndarray
function luandarray.reshape(array, new_shape) end

---@param array ln.Ndarray
---@return ln.Ndarray
function luandarray.transpose(array) end

---@param array ln.Ndarray
---@return ln.Ndarray
function luandarray.flatten(array) end

---@param array ln.Ndarray
---@return string
function luandarray.tostring(array) end

---@param value number
---@param shape table
---@return ln.Ndarray
function luandarray.full(value, shape) end

---@param shape table
---@return ln.Ndarray
function luandarray.zeros(shape) end

---@param array ln.Ndarray
function luandarray.NdarrayDelete(array) end

---@type LuaNdarray
local ln = require "cinit"

return ln