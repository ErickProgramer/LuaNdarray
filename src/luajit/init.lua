-- imports --
local type=type
local setmetatable=setmetatable
local getmetatable=getmetatable
local rawset=rawset
local rawget=rawget
local newproxy=newproxy
local table=table

local ffi=require"ffi"
local newDType, dtype_meta = require("luandarray.luajit.dtype").newDType, require("luandarray.luajit.dtype").dtype_meta
local Complex = require("luandarray.luajit.complex")

-- load definitions.
require "luandarray.luajit.defs"

local lnc=ffi.load("luandarray/luajit/core")
lnc.LNDType_Init()

local proxy_key = rawget(_G, "__GC_PROXY") or "__gc_proxy"

local function ln_setmetatable(t, mt)
    if mt ~= nil and rawget(mt, "__gc") and not rawget(t,"__gc_proxy") then
        local p = newproxy(true)
        rawset(t, proxy_key, p)
        getmetatable(p).__gc = function()
            rawset(t, proxy_key, nil)
            local nmt = getmetatable(t)
            if not nmt then return end
            local fin = rawget(nmt, "__gc")
            if fin then return fin(t) end
        end
    end
    return setmetatable(t,mt)
end


local function LNError_Ocurred()
    if lnc.LNError_Ocurred()==1 then
        return true
    end
    return false
end

local function LNError_Get()
    local errmsg_c = lnc.LNError_Get()
    local errmsg = ""

    local i=0
    while errmsg_c[i] ~= 0 do
        errmsg=errmsg..string.char(errmsg_c[i])
        i=i+1
    end

    return errmsg
end

local function LNError_Raise(level)
    error(LNError_Get(), (level or 1)+1)
end

local function string_to_charvec(str)
    local vec=ffi.new("char[?]", #str+1)
    for i = 1, #str do
        vec[i-1]=string.byte(str:sub(i,i))
    end
    vec[#str]=0
    return vec
end

local function isFloat(x)
    return type(x) == "number" and x ~= math.floor(x)
end

local function isInteger(x)
    return type(x)=="number" and x == math.floor(x)
end

local ln={
    _VERSION="2.0.0",
    MAJOR=2,
    MIJOR=0,
    PATCH=0
}

local function assert(v, message, level)
    message = message or "assertion failed!"
    level = level or 1

    if not v then
        error(message, level)
    end
end

local function expected_arg(fname, v, arg, expected)
    assert(ln.type(arg)=="ndarray", ("bad argument #%d to '%s' (%s expected, got %s)"):format(arg, fname, expected, ln.type(v)), 2)
end

local LNGetDTypeByID
do
    local id_dtype={
        [lnc.LN_INT8]=ln.int8,
        [lnc.LN_INT16]=ln.int16,
        [lnc.LN_INT32]=ln.int32,
        [lnc.LN_INT64]=ln.int32,

        [lnc.LN_UINT8]=ln.uint8,
        [lnc.LN_UINT16]=ln.uint16,
        [lnc.LN_UINT32]=ln.uint32,
        [lnc.LN_UINT64]=ln.uint32,

        [lnc.LN_FLOAT32]=ln.float32,
        [lnc.LN_FLOAT64]=ln.float64,

        [lnc.LN_COMPLEX64]=ln.complex64,
        [lnc.LN_COMPLEX128]=ln.complex128,

        [lnc.LN_BOOL]=ln.bool,
        [lnc.LN_CHAR]=ln.char
    }
    function LNGetDTypeByID(id)
        return id_dtype[id]
    end
end

function dtype_meta:__call(value)
    return ln.array(value, self)
end

---@class ln.Ndarray*
---@field dimensions ffi.cdata*
---@field strides ffi.cdata*
---@field dtype ln.dtype*
---@field size ffi.cdata*
---@field nd ffi.cdata*

---@class ln.Ndarray
---@field shape table
---@field strides table
---@field ndim integer
---@field size integer
---@operator add(ln.Ndarray):ln.Ndarray
---@operator sub(ln.Ndarray):ln.Ndarray
---@operator div(ln.Ndarray):ln.Ndarray
---@operator mul(ln.Ndarray):ln.Ndarray
---@operator pow(ln.Ndarray):ln.Ndarray
---@field private ndarray ln.Ndarray*
local ndarray={
    shape={},
    strides={},
    ndim=0,
    size=0,
}
ndarray.__name = "ndarray"

function ndarray:__index(idx)
    if type(idx)=="number" then
        local res=lnc.LNArray_Index(self.ndarray, idx)
        if LNError_Ocurred() then
            LNError_Raise()
        end
        return ln.ndarray(res)
    elseif idx=="ndim" then
        return self.ndarray.nd
    end
    return rawget(ndarray, idx)
end

---@overload fun(): ln.Ndarray
---@overload fun(ndarray: ln.Ndarray*): ln.Ndarray
---@overload fun(shape: table, dtype: ln.dtype, strides?: table): ln.Ndarray
function ln.ndarray(...)
    local args={...}

    if #args == 0 then
        return ndarray
    end

    local self = ln_setmetatable({}, ndarray)

    if #args == 1 and type(args[1])=="cdata" then
        ---@type ln.Ndarray*
        local cndarray = args[1]

        self.ndarray=cndarray

        self.ndim = tonumber(cndarray.nd)
        self.shape = {}
        self.strides = {}
        self.dtype = LNGetDTypeByID(cndarray.dtype.id)

        self.size = 1
        for i = 1, self.ndim do
            self.shape[i] = cndarray.dimensions[i-1]
            self.strides[i] = cndarray.strides[i-1]
            self.size = self.size * self.shape[i]
        end

        return self
    elseif #args == 2 then
        self.shape, self.dtype = args[1], args[2]
        self.ndim = #self.shape

        self.size = 1
        local shape = ffi.new("size_t[?]", self.ndim)
        for i = 1, self.ndim do
            shape[i-1] = self.shape[i]
            self.size = self.size * shape[i-1]
        end

        self.ndarray = lnc.LNArray_Empty(shape, self.ndim, self.dtype.dtype)
        if LNError_Ocurred() then
            LNError_Raise()
        end

        return self
    elseif #args == 3 then
        self.shape, self.dtype, self.strides = args[1], args[2], args[3]
        self.ndim = #self.shape

        local shape = ffi.new("size_t[?]", self.ndim)
        for i = 1, self.ndim do
            shape[i-1] = self.shape[i]
        end
        self.ndarray = lnc.LNArray_Empty(shape, self.ndim, self.dtype.dtype)

        if not self.strides then
            self.strides = {}
            for i = 1, self.ndim do
                self.strides[i] = self.ndarray.strides[i-1]
            end
        else
            for i = 1, self.ndim do
                self.ndarray.strides[i-1] = self.strides[i]
            end
        end

        return self
    else
        error("wrong number of arguments to 'ndarray'", 2)
    end
end

function ndarray:astype(new)
    local res = lnc.LNArray_CastTo(self.ndarray, new.dtype)
    if LNError_Ocurred() then
        LNError_Raise()
    end

    return ln.ndarray(res)
end

function ndarray:item(idx)
    local dtype=self.dtype.name.."_t*"
    if self.dtype.name=="char" then
        dtype="char*"
    end
    if idx then
        return tonumber(ffi.cast(dtype, self.ndarray.data)[idx])
    else
        assert(self.size==1, "can only convert an array of size 1 to Lua scalar")
        return tonumber(ffi.cast(dtype, self.ndarray.data)[0])
    end
end

local function recursive_totable(out,a)
    if a.ndim==0 then
        table.insert(out, a:item())
        return
    end
    for i, v in ln.iter(a) do
        recursive_totable(out,v)
    end
end

function ndarray:totable()
    local res={}
    recursive_totable(res,self)
    return res
end

function ndarray:__tostring()
    local str_c= lnc.LNArray_toString(self.ndarray,string_to_charvec"array(",string_to_charvec")")
    local str=""

    local i=0
    while str_c[i]~=0 do
        str=str..string.char(str_c[i])
        i=i+1
    end

    return str
end

function ndarray:__gc()
    lnc.LNArray_Free(self.ndarray)
    self = nil
end

function ndarray:__add(other)
    return ln.add(self,other)
end

ndarray.iter = ln.iter

---@param data table|number
---@param dtype? ln.dtype
---@return ln.Ndarray
function ln.array(data, dtype)
    local flatten_data={}
    local function flatten(x)
        if ln.type(x)~="table" or Complex.is(x) then
            table.insert(flatten_data,x)
            return
        end

        for i = 1, #x do
            flatten(x[i])
        end
    end
    flatten(data)

    if not dtype then
        for i = 1, #flatten_data do
            if Complex.is(flatten_data[i]) then
                dtype = ln.complex128
                break
            elseif isFloat(flatten_data[i]) then
                dtype = ln.float64
                break
            elseif isInteger(flatten_data[i]) then
                dtype = ln.int64
                break
            elseif type(flatten_data[i]) == "boolean" then
                dtype = ln.bool
                break
            end
        end
    end

    local shape={}
    while type(data)=="table" and data.__name ~= "complex" do
        table.insert(shape,#data)
        data=data[1]
    end

    local dtype_repr = dtype.name .. "_t[?]"
    local data_c = ffi.new(dtype_repr, #flatten_data)

    if dtype == ln.complex128 or dtype == ln.complex64 then
        for i = 1, #flatten_data do
            if Complex.is(flatten_data[i]) then
                data_c[i-1].realp = flatten_data[i].real
                data_c[i-1].imagp = flatten_data[i].imag
            elseif type(flatten_data[i]) == "number" then
                data_c[i-1].realp = flatten_data[i]
                data_c[i-1].imagp = 0
            end
        end
    else
        for i = 1, #flatten_data do
            data_c[i-1] = flatten_data[i]
        end
    end

    local arr = ln.ndarray(shape, dtype)
    arr.ndarray.data = ffi.cast("char*", data_c)

    return arr
end

function ln.iter(arr)
    assert(ln.type(arr) == "ndarray", "bad argument #1 to 'iter' (ndarray expected, got "..ln.type(arr)..")", 2)

    local i = -1
    return function ()
        i = i + 1
        if i == arr.shape[1] then
            return nil, nil
        end

        return i, arr[i]
    end
end

---@param shape? table
---@param dtype? ln.dtype
---@return ln.Ndarray
function ln.zeros(shape, dtype)
    if shape == nil then
        shape = {}
    elseif type(shape) == "number" then
        shape = {shape}
    end

    dtype=dtype or ln.float64

    local dims=ffi.new("size_t[?]", #shape)
    for i = 1, #shape do
        dims[i-1] = shape[i]
    end

    local res=lnc.LNArray_Zeros(dims,#shape,dtype.dtype)
    if LNError_Ocurred() then
        LNError_Raise()
    end

    return ln.ndarray(res)
end

---@param shape table
---@param dtype? ln.dtype
---@return table
function ln.ones(shape, dtype)
    dtype = dtype or ln.float64

    local dims=ffi.new("size_t[?]", #shape)
    for i = 1, #shape do
        dims[i-1] = shape[i]
    end

    local res=lnc.LNArray_Ones(dims,#shape,dtype.dtype)
    if LNError_Ocurred() then
        LNError_Raise()
    end

    return ln.ndarray(res)
end

---@param shape table
---@param dtype? ln.dtype
---@return ln.Ndarray
function ln.empty(shape, dtype)
    dtype = dtype or ln.float64

    local dims=ffi.new("size_t[?]", #shape)
    for i = 1, #shape do
        dims[i-1] = shape[i]
    end

    local res=lnc.LNArray_Empty(dims,#shape,dtype.dtype)
    if LNError_Ocurred() then
        LNError_Raise()
    end

    return ln.ndarray(res)
end

---@param arr ln.Ndarray
---@param to table
---@return ln.Ndarray
function ln.broadcast_to(arr, to)
    ---@type ln.Ndarray*
    local out = ffi.cast("Ndarray*", lnc.LNMem_alloc(ffi.sizeof("Ndarray")))
    if LNError_Ocurred() then
        LNError_Raise()
    end

    local to_c = ffi.new("size_t[?]", #to)
    for i = 1, #to do
        to_c[i-1]=to[i]
    end

    lnc.LNArray_BroadcastTo(out, arr.ndarray, to_c, #to, nil)
    if LNError_Ocurred() then
        LNError_Raise()
    end

    return ln.ndarray(out)
end

function ln.add(arr1,arr2)
    local res = lnc.LNArray_Add(arr1.ndarray, arr2.ndarray)
    if LNError_Ocurred() then
        LNError_Raise()
    end

    return ln.ndarray(res)
end

function ln.max(arr, axis)
    local res
    if axis then
        res=lnc.LNArray_MaxAxis(arr.ndarray, axis)
    else
        res=lnc.LNArray_Max(arr.ndarray)
    end
    if LNError_Ocurred() then
        LNError_Raise()
    end
    return ln.ndarray(res)
end

function ln.promote_types(t1, t2)
    if(t1.dtype.id > t2.dtype.id) then
        if (t1.dtype.id>=lnc.LN_UINT8 and t1.dtype.id<=lnc.LN_UINT64) and
           (t2.dtype.id>=lnc.LN_INT8 and t2.dtype.id<=lnc.LN_INT64)then    
            return ln.float64
        end
        return t1
    else
        if (t2.dtype.id>=lnc.LN_UINT8 and t2.dtype.id<=lnc.LN_UINT64) and
           (t1.dtype.id>=lnc.LN_INT8 and t1.dtype.id<=lnc.LN_INT64)then    
            return ln.float64
        end
        return t2
    end
end

do
    ---@param self ln.dtype
    ---@param data any
    ---@return ln.Ndarray
    local function builder(self, data)
        return ln.array(data, self)
    end

    ln.int8=newDType(lnc.LNInt8, builder)

    ln.int16=newDType(lnc.LNInt16, builder)
    ln.int32=newDType(lnc.LNInt32, builder)
    ln.int64=newDType(lnc.LNInt64, builder)

    ln.uint8=newDType(lnc.LNUInt8, builder)
    ln.uint16=newDType(lnc.LNUInt16, builder)
    ln.uint32=newDType(lnc.LNUInt32, builder)
    ln.uint64=newDType(lnc.LNUInt64, builder)

    ln.float32=newDType(lnc.LNFloat32, builder)
    ln.float64=newDType(lnc.LNFloat64, builder)

    ln.complex64=newDType(lnc.LNComplex64, builder)
    ln.complex128=newDType(lnc.LNComplex128, builder)

    ln.bool=newDType(lnc.LNBool, builder)
    ln.char=newDType(lnc.LNChar, builder)

    ln.byte=ln.int8
end

do
    local function isNdarray(x)
        return type(x) == "table" and
            x.ndarray and tostring(ffi.typeof(x.ndarray)) == "ctype<struct Ndarray_fields *>" and
            type(x.shape) == "table" and
            type(x.strides) == "table" and
            type(x.ndim) == "number" and
            type(x.size) == "number" and
            getmetatable(x) == ndarray
    end


    --- Simillar to Lua's standard `type` function but, with `ndarray` and `dtype`
    ---@nodiscard
    ---@alias ln.type "ndarray"|"dtype"|"thread"|"cdata"|"userdata"|"table"|"function"|"string"|"number"|"boolean"|"nil"
    ---@overload fun(v: any): ln.type
    ---@return ln.type
    function ln.type(...)
        if #({...}) > 1 then
            error("wrong number of arguments to 'type'")
        end

        local v = ...
        if isNdarray(v) then
            return "ndarray"
        elseif dtype_meta.is(v) then
            return "dtype"
        end
        return type(v)
    end
end

return ln