--              LuaJIT                --
-- LuaNdarray Complex Numbers library --

local setmetatable = setmetatable
local type = type
local math = math

local function re(x)
    if type(x)=="number" then
        return x
    end
    return x.real
end

local function im(x)
    if type(x)=="number" then
        return 0
    end
    return x.imag
end

local complex={}
complex.__index=complex
complex.__name="complex"

local function Complex(real, imag)
    local self=setmetatable({},complex)
    self.real = real
    self.imag = imag
    return self
end

local function rect(r,phi)
	return Complex(r*math.cos(phi),r*math.sin(phi))
end

function complex:__add(other)
    return Complex(re(self)+re(other), im(self)+im(other))
end

function complex:__sub(other)
    return Complex(re(self)-re(other), im(self)-im(other))
end

function complex:__mul(other)
    local r1,i1, r2,i2 = re(self),im(self), re(other),im(other)
    return Complex(r1*r2-i1*i2,r1*i2+r2*i1)
end

function complex:__div(other)
    local r1,i1, r2,i2=re(self),im(self), re(other),im(other)
    local rsq=r2^2+i2^2
    return Complex((r1*r2+i1*i2)/rsq,(r2*i1-r1*i2)/rsq)
end

function complex:__pow(other)
    local r1,i1, r2,i2=re(self),im(self), re(other),im(other)
    local rsq=r1^2+i1^2
    if rsq==0 then
        if r2==0 and i2==0 then
            return 1
        end
        return 0
    end
    local phi=math.atan2(i1,r1)
    return rect(rsq^(r2/2)*math.exp(-i2*phi),i2*math.log(rsq)/2+r2*phi)
end

function complex:__unm()
    return Complex(-re(self), -im(self))
end

function complex:__tostring()
    local r,i=re(self),im(self)
    if i==0 then
        return tostring(r)
    elseif r==0 then
        if i==1 then
            return "i"
        elseif i==-1 then
            return "-i"
        end
        return i.."i"
    elseif i<0 then
        if i==-1 then
            return r.."-i"
        end
        return r..i.."i"
    else
        if i==1 then
            return r.."+i"
        end
        return r.."+"..i.."i"
    end
end

function complex:is()
    return (type(self)=="table" and self.__name=="complex")
end

-- global Complex to create Complex numbers...
I = Complex(0,1)

complex.new=Complex

return complex