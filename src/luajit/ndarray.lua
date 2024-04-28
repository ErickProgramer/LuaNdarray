-- Ndarray Object --

local ndarray = {}
ndarray.__index=ndarray

function ndarray.new(...)
    if #({...}) == 1 then
        return setmetatable(..., ndarray)
    end

    if #({...}) > 3 then
        error("wrong number of arguments to 'ndarray'",2)
    end

    local self = setmetatable({}, ndarray)
    local data, shape, dtype = ...
end
