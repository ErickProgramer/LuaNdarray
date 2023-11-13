if jit then error "LuaNdarray dont have support for LuaJit per while" end

---@diagnostic disable: missing-return

---@class ln.Ndarray
---@operator len(ln.Ndarray):integer
---@operator add(ln.Ndarray):ln.Ndarray
---@operator sub(ln.Ndarray):ln.Ndarray
---@operator mul(ln.Ndarray):ln.Ndarray
---@operator div(ln.Ndarray):ln.Ndarray
---@operator pow(ln.Ndarray):ln.Ndarray
---@field shape table
---@field size number
---@field ndim number
local Ndarray = {}

function Ndarray:print() end

---@return ln.Ndarray
function Ndarray:select(idx) end

---@overload fun():ln.Ndarray
---@param i integer
---@return ln.Ndarray
function Ndarray:item(i) end

---@param new_shape table
---@return ln.Ndarray
function Ndarray:reshape(new_shape) end

---@class LuaNdarray
---@field __VERSION string current version of LuaNdarray represented by a string
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

---@param shape table
---@return ln.Ndarray
function luandarray.ones(shape) end

---@param array ln.Ndarray
function luandarray.NdarrayDelete(array) end

---@param arr1 ln.Ndarray
---@param arr2 ln.Ndarray
---@return ln.Ndarray
function luandarray.add(arr1, arr2) end

---@param arr1 ln.Ndarray
---@param arr2 ln.Ndarray
---@return ln.Ndarray
function luandarray.sub(arr1, arr2) end

---@param arr1 ln.Ndarray
---@param arr2 ln.Ndarray
---@return ln.Ndarray
function luandarray.mul(arr1, arr2) end

---@param arr1 ln.Ndarray
---@param arr2 ln.Ndarray
---@return ln.Ndarray
function luandarray.div(arr1, arr2) end

---@param arr1 ln.Ndarray
---@param arr2 ln.Ndarray
---@return ln.Ndarray
function luandarray.pow(arr1, arr2) end

---@param x ln.Ndarray | number
---@return ln.Ndarray | number
function luandarray.acos(x) end

---@param x ln.Ndarray | number
---@return ln.Ndarray | number
function luandarray.asin(x) end

---@param x ln.Ndarray | number
---@return ln.Ndarray | number
function luandarray.atan(x) end

---@param x ln.Ndarray | number
---@return ln.Ndarray | number
function luandarray.ceil(x) end

---@param x ln.Ndarray | number
---@return ln.Ndarray | number
function luandarray.cos(x) end

---@param x ln.Ndarray | number
---@return ln.Ndarray | number
function luandarray.cosh(x) end

---@param x ln.Ndarray | number
---@return ln.Ndarray | number
function luandarray.exp(x) end

---@param x ln.Ndarray | number
---@return ln.Ndarray | number
function luandarray.floor(x) end

---@param x ln.Ndarray | number
---@return ln.Ndarray | number
function luandarray.log10(x) end

---@param x ln.Ndarray | number
---@return ln.Ndarray | number
function luandarray.log(x) end

---@param x ln.Ndarray | number
---@return ln.Ndarray | number
function luandarray.sin(x) end

---@param x ln.Ndarray | number
---@return ln.Ndarray | number
function luandarray.sinh(x) end

---@param x ln.Ndarray | number
---@return ln.Ndarray | number
function luandarray.sqrt(x) end

---@param x ln.Ndarray | number
---@return ln.Ndarray | number
function luandarray.tan(x) end

---@param x ln.Ndarray | number
---@return ln.Ndarray | number
function luandarray.tanh(x) end

---@param start boolean
function luandarray.IndexStartZero(start) end

---@return integer
function luandarray.GetIndexStart() end


---@type LuaNdarray
local ln = require "cinit"

return ln