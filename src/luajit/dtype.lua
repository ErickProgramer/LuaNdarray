local setmetatable, getmetatable = setmetatable, getmetatable
local type = type

local ffi=require"ffi"

local lnc=ffi.load("luandarray/luajit/core")

local dtype_meta={}
dtype_meta.__name = "dtype"
dtype_meta.__index=dtype_meta

---@return ln.dtype
local function newDType(struct, constructor)
    local self=setmetatable({},dtype_meta)
    getmetatable(self).__call = constructor

    self.dtype=struct
    self.name=""
    self.alignment=self.dtype.alignment
    self.itemsize=self.dtype.itemsize

    local i=0
    while self.dtype.name[i]~=0 do
        self.name=self.name..string.char(self.dtype.name[i])
        i=i+1
    end
    return self
end

function dtype_meta:__tostring()
    return string.format('dtype("%s")', self.name)
end

function dtype_meta:__gc()
    lnc.LNDType_Free(self.dtype)
    self=nil
end

function dtype_meta.is(v)
    return type(v) == "table" and (v.__name=="dtype" and tostring(ffi.typeof(v.dtype)) == "ctype<const struct LNTypeDescr *>")
end

return {newDType=newDType, dtype_meta=dtype_meta}