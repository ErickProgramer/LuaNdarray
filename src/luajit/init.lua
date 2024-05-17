-- imports --
local type=type
local setmetatable=setmetatable
local getmetatable=getmetatable
local rawset=rawset
local rawget=rawget
local newproxy=newproxy
local table=table
local math=math

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

---@param n integer
---@return ffi.cdata*
local function alloc(tpy, n)
    local block = lnc.LNMem_alloc(n * ffi.sizeof(tpy))
    if LNError_Ocurred() then
        LNError_Raise(2)
    end

    return ffi.cast(tpy.."*", block)
end

local function string_to_charvec(str)
    local vec = alloc("char", #str+1)
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

ln.I = I

local IsValidCastRule
local ConvertRuleNameToID
do
    local cast_rules = {"safe", "unsafe"}
    local cast_rules_id = {
        safe = 1,
        unsafe =0
    }
    function IsValidCastRule(v)
        for i = 1, #cast_rules do
            if v == cast_rules[i] then
                return true
            end
        end
        return false
    end

    function ConvertRuleNameToID(v)
        return cast_rules_id[v]
    end
end

local function assert(v, message, level)
    message = message or "assertion failed!"
    level = level or 1

    if not v then
        error(message, level)
    end
end

local f = string.format

---@param fname string
---@param v any
---@param arg integer
---@param expected table<ln.typef>|ln.typef
---@param level? integer
local function expected_arg(fname, v, arg, expected, level)
    if type(expected) ~= "table" then
        expected = {expected}
    end
    level = level or 3
    
    local is_expected = false
    for i = 1, #expected do
        if expected[i] == "integer" or expected[i] == "float" then
            if ln.typef(v) == expected[i] then
                is_expected = true
                break
            end
        else
            if ln.type(v) == expected[i] then
                is_expected = true
                break
            end
        end
    end

    if not is_expected then
        error(f("bad argument #%d to '%s' (%s expected, got %s)", arg, fname, table.concat(expected, " or "), ln.type(v)), level)
    end
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
    elseif idx == "strides" then
        local strides = setmetatable({},{
            __tostring = function (t)
                return "Strides({"..table.concat(t, ", ").."})"
            end
        })

        for i = 1, self.ndim do
            strides[i] = tonumber(self.ndarray.strides[i-1])
        end

        return strides
    elseif idx == "shape" then
        local shape = setmetatable({},{
            __tostring = function (t)
                return "Shape({" .. table.concat(t, ", ") .. "})"
            end,
        })

        for i = 1, self.ndim do
            shape[i] = tonumber(self.ndarray.dimensions[i-1])
        end

        return shape
    elseif idx=="ndim" then
        return self.ndarray.nd
    end
    return rawget(ndarray, idx)
end

---@overload fun(): ln.Ndarray
---@overload fun(ndarray: ln.Ndarray*): ln.Ndarray
---@overload fun(shape: table, dtype: ln.dtype): ln.Ndarray
---@overload fun(shape: table, dtype: ln.dtype, strides: table): ln.Ndarray
function ln.ndarray(...)
    local args={...}

    if #args == 0 then
        return ndarray
    end

    local self = ln_setmetatable({}, ndarray)

    if #args == 1 then
        ---@type ln.Ndarray*
        local cndarray = args[1]

        assert(ln.type(cndarray) == "ndarray*", "bad argument #1 to 'ndarray' (expected ndarray pointer, got " .. ln.type(cndarray) .. ")")

        self.ndarray=cndarray

        self.ndim = tonumber(cndarray.nd)
        self.dtype = LNGetDTypeByID(cndarray.dtype.id)

        return self
    elseif #args == 2 then
        self.shape, self.dtype = args[1], args[2]
        assert(ln.typef(self.shape) == "table" or ln.typef(self.shape) == "integer",
               "bad argument #1 to 'ndarray' (ndarray or integer expected, got " .. ln.typef(self.shape) .. ")", 2)
        assert(ln.type(self.dtype) == "dtype",
               "bad argument #2 to 'ndarray' (dtype expected, got " .. ln.type(self.dtype[2])..")", 2)
        self.ndim = #self.shape

        if ln.type(self.shape) == "number" then
            self.shape = {self.shape}
        else
            for i = 1, self.ndim do
                assert(ln.typef(self.shape[i]) == "integer", "bad argument #2 to 'ndarray' (expected a sequence of integers", 2)
            end
        end


        self.size = 1
        local shape = alloc("size_t", self.ndim)
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
        assert(ln.typef(self.shape) == "table" or ln.typef(self.shape) == "integer",
               "bad argument #1 to 'ndarray' (ndarray or integer expected, got " .. ln.typef(self.shape) .. ")", 2)
        assert(ln.type(self.dtype) == "ndarray",
               "bad argument #2 to 'ndarray' (ndarray expected, got " .. ln.type(self.dtype[2])..")", 2)
        assert(ln.typef(self.strides) == "ndarray" or ln.typef(self.strides) == "integer",
              "bad argument #3 to 'ndarray' (ndarray or integer expected, got " .. ln.typef(self.strides) .. ")", 2)
        assert(#self.shape == #self.strides,
                "shape (#1) and strides (#3) must has the same length", 2)

        self.ndim = #self.shape
        if ln.type(self.shape) == "number" then
            self.shape = {self.shape}
        else
            for i = 1, self.ndim do
                assert(ln.typef(self.shape[1]) == "integer",
                       "bad argument #1 to 'ndarray' (expected a sequence of integers)", 2)
                assert(ln.typef(self.strides[i]) == "integer",
                       "bad argument #3 to 'ndarray' (expected a sequence of integers)", 2)
            end
        end

        local shape = alloc("size_t", self.ndim)
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

---@overload fun(self: ln.Ndarray, new: ln.dtype): ln.Ndarray
---@overload fun(self: ln.Ndarray, new: ln.dtype, casttype: "safe"|"unsafe"): ln.Ndarray
function ndarray:astype(...)
    assert(#({...}) >= 2 or #({...}) <= 3,
          "wrong number of argumnts to 'astype'", 2)
    assert(ln.type(self) == "ndarray",
          "bad argument #1 to 'astype' (ndarray expected, got " .. ln.type(self) .. ")", 2)

    local new, casttype = ...
    assert(ln.type(new) == "dtype",
          "bad argument #1 to 'astype' (dtype expected, got " .. ln.type(new) .. ")")

    if casttype ~= nil then
        assert(ln.type(casttype) == "string",
              "bad argument #2 to 'astype' (string expected, got " .. ln.type(casttype) .. ")",
                2)
        assert(IsValidCastRule(casttype),
               "bad argument #2 to 'astype' (invalid mode)", 2)
        if casttype == "safe" then
            casttype = lnc.LNCAST_SAFE
        else
            casttype = lnc.LNCAST_UNSAFE
        end
    else
        casttype = lnc.LNCAST_UNSAFE
    end

    local res = lnc.LNArray_CastTo(self.ndarray, new.dtype, casttype)
    if LNError_Ocurred() then
        LNError_Raise()
    end

    return ln.ndarray(res)
end

---@overload fun(self: ln.Ndarray): number?
---@overload fun(self: ln.Ndarray, idx: integer): number?
function ndarray:item(...)
    local args = {...}

    local dtype = self.dtype.name.."_t*"
    if self.dtype.name == "char" then
        dtype = "char*"
    end

    if #args == 0 then
        assert(self.size==1, "can only convert an array of size 1 to Lua scalar", 2)
        return tonumber(ffi.cast(dtype, self.ndarray.data)[0])
    elseif #args == 1 then
        local idx = args[1]
        assert(ln.typef(idx) == "integer", "bad argument #1 to 'item' (integer expected, got " .. ln.typef(idx) .. ")", 2)
        return tonumber(ffi.cast(dtype, self.ndarray.data)[idx]) or nil
    end

    error("wrong number of arguments to 'item'", 2)
end

do
    local function recursive_totable(out,a)
        if a.ndim==0 then
            table.insert(out, a:item())
            return
        end
        for i, v in ln.iter(a) do
            recursive_totable(out,v)
        end
    end

    function ndarray:totable(...)
        assert(#({...}) == 0, "wrong number of arguments to 'totable'", 2)
        local res={}
        recursive_totable(res,self)
        return res
    end
end

function ndarray:__tostring()
    local str_c= lnc.LNArray_toString(self.ndarray,string_to_charvec"array(",string_to_charvec")")
    if LNError_Ocurred() then
        LNError_Raise()
    end

    local str=""
    local i=0
    while str_c[i]~=0 do
        str=str..string.char(str_c[i])
        i = i + 1
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

do
    local function flatten(dst, src, dim)
        if ln.type(src) ~= "table" then
            table.insert(dst, src)
            return
        end

        for i = 2, #src do
            if ln.type(src[i]) == "number" or ln.type(src[i]) == "complex" then
                assert(ln.type(src[1]) == "complex" or ln.type(src[1]) == "number", "inhomogenous shape after " .. dim .. " dimension")
            else
                assert(#src[i] == #src[1], "inhomogenous shape after " .. dim .. " dimension")
            end
        end

        for i = 1, #src do
            flatten(dst, src[i], dim+1)
        end
    end

    ---@overload fun(data: any, dtype?: ln.dtype): ln.Ndarray
    function ln.array(...)
        assert(#({...}) == 2 or #({...}) == 1, "wrong number of arguments to 'array'")

        local data, dtype = ...

        local data_flat = {}
        flatten(data_flat, data, 0)
        local size = #data_flat

        -- select automaticcly
        if dtype == nil then
            dtype = ln.float64
            for i = 1, size do
                if Complex.is(data[i]) then
                    dtype = ln.complex128
                    break
                elseif isFloat(data[i]) then
                    dtype = ln.float64
                    break
                elseif isInteger(data[i]) then
                    dtype = ln.int64
                    break
                elseif ln.type(data[i]) == "boolean" then
                    dtype = ln.bool
                    break
                end
            end
        else
            assert(ln.type(dtype) == "dtype", "bad argument #2 to 'array (dtype expected, got " .. ln.type(dtype) .. ")", 2)
        end


        local ndim = 0
        local shape = {}
        while ln.type(data) == "table" do
            ndim = ndim + 1
            shape[ndim] = #data
            data = data[1]
        end

        local dims = ffi.cast("size_t*", alloc("size_t", ndim))
        for i = 1, ndim do
            dims[i-1] = shape[i]
        end

        local repr = dtype.name .. "_t"
        local cdata = alloc(repr, dtype.alignment * #data_flat)

        if dtype == ln.complex128 or dtype == ln.complex64 then
            for i=1, size do
                if Complex.is(data_flat[i]) then
                    cdata[i-1].realp = data_flat[i].real
                    cdata[i-1].imagp = data_flat[i].imag
                else
                    cdata[i-1].realp = data_flat[i]
                    cdata[i-1].imagp = 0.0
                end
            end
        else
            for i = 1, size do
                if Complex.is(data_flat[i]) then
                    cdata_cast[i-1] = data_flat[i].real
                else
                    cdata_cast[i-1] = data_flat[i]
                end
            end
        end
        local res = lnc.LNArray_New(cdata, dims, nil, ndim, dtype.dtype)

        return ln.ndarray(res)
    end

end

---@overload fun(arr: ln.Ndarray): fun(): integer, ln.Ndarray
function ln.iter(...)
    assert(#({...}) == 0, "wrong number of arguments to 'iter'", 2)
    local arr = ...
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

---@overload fun(shape?: table<integer, integer> | integer, dtype?: ln.dtype): ln.Ndarray
function ln.zeros(...)
    assert(#({...}) == 2 or #({...}) == 1, "wrong number of argumentos to 'zeros'", 2)

    local shape, dtype = ...
    expected_arg("zeros", shape, 1, {"table", "integer", "nil"})

    if dtype ~= nil then
        expected_arg("zeros", dtype, 2, "dtype")
    else
        dtype = ln.float64
    end

    if shape == nil then
        shape = {}
    elseif type(shape) == "number" then
        shape = {shape}
    end


    local dims = alloc("size_t", #shape)
    for i = 1, #shape do
        assert(isInteger(shape[i]), "bad argument #1 to 'empty' (expected a sequence of integers)", 2)
        dims[i-1] = shape[i]
    end

    local res=lnc.LNArray_Zeros(dims,#shape,dtype.dtype)
    if LNError_Ocurred() then
        LNError_Raise()
    end

    return ln.ndarray(res)
end

---@overload fun(shape?: table<integer, integer> | integer, dtype?: ln.dtype): ln.Ndarray
function ln.ones(...)
    assert(#({...}) == 2 or #({...}) == 1, "wrong number of arguments to 'ones'", 2)

    local shape, dtype = ...
    expected_arg("ones", shape, 1, {"table", "integer", "nil"})

    if dtype ~= nil then
        expected_arg("ones", dtype, 2, "dtype")
    else
        dtype = ln.float64
    end

    if shape == nil then
        shape = {}
    elseif type(shape) == "number" then
        shape = {shape}
    end

    local dims = alloc("size_t", #shape)
    for i = 1, #shape do
        assert(isInteger(shape[i]), "bad argument #1 to 'ones' (expected a sequence of integers)", 2)
        dims[i-1] = shape[i]
    end

    local res=lnc.LNArray_Ones(dims,#shape,dtype.dtype)
    if LNError_Ocurred() then
        LNError_Raise()
    end

    return ln.ndarray(res)
end

---@overload fun(shape?: table<integer, integer> | integer, dtype?: ln.dtype): ln.Ndarray
function ln.empty(...)
    assert(#({...}) == 2 or #({...}) == 1, "wrong number of arguments to 'empty'", 2)

    local shape, dtype = ...
    expected_arg("empty", shape, 1, {"table", "integer"})
    if ln.type(shape) == "number" then
        shape = {shape}
    end

    if dtype ~= nil then
        expected_arg("empty", dtype, 2, "dtype")
    else
        dtype = ln.float64
    end

    if shape == nil then
        shape = {}
    elseif type(shape) == "number" then
        shape = {shape}
    end

    local dims = alloc("size_t", #shape)
    for i = 1, #shape do
        assert(isInteger(shape[i]), "bad argument #1 to 'empty' (expected a sequence of integers)", 2)
        dims[i-1] = shape[i]
    end

    local res=lnc.LNArray_Empty(dims,#shape,dtype.dtype)
    if LNError_Ocurred() then
        LNError_Raise()
    end

    return ln.ndarray(res)
end

---@overload fun(arr: ln.Ndarray, to: table)
function ln.broadcast_to(...)
    if #({...}) ~= 2 then
        error("wrong number of arguments to 'broadcast_to'", 2)
    end

    local arr, to = ...
    ---@type ln.Ndarray*
    local out = alloc("Ndarray", 1)

    local to_c = alloc("size_t", #to)
    for i = 1, #to do
        to_c[i-1]=to[i]
    end

    lnc.LNArray_BroadcastTo(out, arr.ndarray, to_c, #to)
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

do
    ---@overload fun(self: ln.dtype, data: any)
    local function builder(...)
        local dtype, data = ...
        if #({...}) > 2 then
            error("wrong number of arguments to '" .. dtype.name.."'")
        end

        return ln.array(data, dtype)
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

    --        aliases        --

    ln.byte     = ln.int8
    ln.int      = ln.int32
    ln.complex  = ln.complex128
    ln.float    = ln.float64
    ln.double   = ln.float64
    ln.longlong = ln.int64
    ln.long     = ln.int32
    ln.short    = ln.int16

    ---------------------------
end

do
    local function isNdarray(x)
        return type(x) == "table" and
            x.ndarray and tostring(ffi.typeof(x.ndarray)) == "ctype<struct Ndarray_fields *>" and
            type(x.shape) == "table" and
            type(x.strides) == "table" and
            isInteger(x.ndim) and
            isInteger(x.size) and
            getmetatable(x) == ndarray
    end

    local function isNdarrayp(x)
        return type(x) == "cdata" and tostring(ffi.typeof(x)) == "ctype<struct Ndarray_fields *>"
    end

    local function isDTypep(x)
        return type(x) == "cdata" and tostring(ffi.typeof(x) == "ctype<const struct LNTypeDescr *>")
    end

    local _type = type
    --- Simillar to Lua's standard `type` function but, with `ndarray`, `dtype` and others
    ---@nodiscard
    ---@alias ln.type "ndarray"|"ndarray*"|"dtype"|"dtype*"|"complex"|"thread"|"cdata"|"userdata"|"table"|"function"|"string"|"number"|"boolean"|"nil"
    ---@overload fun(v: any): ln.type
    ---@return ln.type
    function ln.type(...)
        if #({...}) > 1 then
            error("wrong number of arguments to 'type'")
        end

        local v = ...
        if isNdarrayp(v) then
            return "ndarray*"
        elseif isNdarray(v) then
            return "ndarray"
        elseif isDTypep(v) then
            return "dtype*"
        elseif dtype_meta.is(v) then
            return "dtype"
        elseif Complex.is(v) then
            return "complex"
        end
        return _type(v)
    end
end

---@param x any
---@alias ln.typef ln.type|"float"|"integer"
---@return ln.typef
function ln.typef(x)
    if isFloat(x) then
        return "float"
    elseif isInteger(x) then
        return "integer"
    end
    return ln.type(x)
end

---@overload fun(from: ln.dtype, to: ln.dtype): boolean
---@overload fun(from: ln.type, to: ln.dtype, casttype: "safe"|"unsafe")
function ln.can_cast(...)
    local from, to, casttype

    local nargs = #({...})
    if nargs == 2 then
        from, to = ...
        casttype = "safe"
    elseif nargs == 3 then
        from, to, casttype = ...
    else
        error("wrong number of arguments to 'can_cast'", 2)
    end

    expected_arg("can_cast", from, 1, "dtype")
    expected_arg("can_cast", to, 1, "dtype")
    expected_arg("can_cast", casttype, 1, "string")

    assert(IsValidCastRule(casttype), "bad argument #3 to 'can_cast' (invalid option)", 2)

    return lnc.LNArray_CanCast(from.dtype, to.dtype, ConvertRuleNameToID(casttype)) == 1
end

return ln