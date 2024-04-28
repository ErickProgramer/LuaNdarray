
local function isspace(s)
    return s:gsub(" ", "")==""
end

local function ischar(char, chars)
    for i = 1, #chars do
        if char==chars:sub(i,i) then
            return true
        end
    end
    return false
end

local function Class(a1,a2)
    local class={}
    class.__class_fields={}

    function class:__tostring()
        local repr=self.__name.."("
        for i=1, #self.__class_fields do
            local k=self.__class_fields[i]
            local v=self[k]

            if k:sub(1,2) ~= "__" then
                repr=repr..k.."="
                if type(v)=="string" then
                    repr=repr..'"'..tostring(v)..'"'
                else
                    repr=repr..tostring(v)
                end
                repr=repr..", "
            end
        end
        repr=repr:sub(1,-3)
        return repr..")"
    end

    if type(a1) == "string" then
        class.__name=a1
        class.__base=a2
    else
        class.__name=a2
        class.__base=a1
    end

    setmetatable(class,{
        __index=class.__base,

        __call=function (cls, ...)
            local self=setmetatable({}, cls)
            self.__class_fields={}
            getmetatable(self).__index= function (t,idx)
                return rawget(t,idx) or rawget(cls, idx) or rawget(rawget(cls, "__base"), idx)
            end
            getmetatable(self).__newindex=function(t,idx,v)
                if idx and not rawget(t,idx) then
                    table.insert(rawget(t,"__class_fields"), idx)
                end
                rawset(t, idx, v)
            end

            cls.__init(self, ...)

            getmetatable(self).__newindex=nil

            return self
        end,

        __tostring=function (cls)
            return "<class '"..cls.__name.."'>"
        end
    })

    return class
end

local Token=Class "Token"

function Token:__init(tok, content, start, stop)
    self.tok=tok
    self.content=content
    self.start=start
    self.stop=stop
end

-- function Token:__tostring()
--     return string.format('Token("%s", %s)', self.tok,self.content)
-- end

local t=Token("import","import",1,5)
print(Token)

-- local function Token(tok, content, start, stop)
--     local self={}

--     self.tok=tok
--     self.content=content or self.tok
--     self.start=start
--     self.stop=stop

--     return setmetatable(self, {
--         __tostring=function (t)
--             return string.format('Token("%s", %s)', t.tok,t.content)
--         end
--     })
-- end

local function items(t)
    local i=0
    local function _items()
        i=i+1
        if t[i]=="nil" then
            return nil
        end
        return t[i]
    end
    return _items
end

local keywords={
    "import", "as", "from", "if", "then", "local", "class", "function", "do", "end"
}

local literals=",.()[]{}\n \"'"

local function Lexer(code)
    local toks={}

    local str_prefix=nil
    local in_str=false
    local str=""

    local identifier=""
    local i=1

    while i <= #code do
        local has_keyword=false
        if not in_str then
            for keyword in items(keywords) do
                local next_chars=code:sub(i, i+#keyword-1)
                if next_chars==keyword and (code:sub(i-1,i-1)==" " or code:sub(i-1,i-1)=="") then
                    table.insert(toks, Token(keyword, next_chars, i, i+#keyword))
                    has_keyword=true
                    i=i+#keyword-1
                    break
                end
            end
        end

        local char=code:sub(i,i)

        if not in_str and ischar(char, '\'"') then
            str_prefix=char
            in_str=true
        elseif in_str and char==str_prefix then
            str_prefix=nil
            in_str=false

            table.insert(toks, Token("string", str, i-#str, i))

            str=""
        elseif in_str then
            str=str..char
        end

        if not in_str then
            if ischar(char,literals) and not ischar(char,'"\'\n ') then
                if not isspace(identifier) then
                    table.insert(toks, Token("identifier", identifier, i-#identifier,i))
                    identifier=""
                end
                table.insert(toks, Token("literal", char, i, i))
            elseif (ischar(char, literals) or has_keyword) and not isspace(identifier)  then
                table.insert(toks, Token("identifier", identifier, i, i+#identifier-1))
                identifier=""
                i=i+#identifier-1
                if ischar(char, literals) then
                    i=i+1
                end
            elseif not has_keyword and not ischar(char,literals) then
                identifier=identifier..char
            end
        end

        i = i + 1
    end
    return toks
end

local code=[[
aimport = 100
]]

local toks=Lexer(code)
for i = 1, #toks do
    print(toks[i])
end

---@class Person
---@operator call(Person):Person
local Person = Class "Person"

function Person:__init(name,age,has_money)
    self.name=name
    self.age=age
    self.has_money=has_money
    if self.has_money then
        self.has_girlfriend=true
    else
        self.has_girlfriend=false
    end
end

local p=Person("Erick",15,true)
print(p+10)

