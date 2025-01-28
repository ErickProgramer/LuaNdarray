

-- MIT License
--
-- Copyright (c) 2023 LuaUtils
--
-- Permission is hereby granted, free of charge, to any person obtaining a copy
-- of this software and associated documentation files (the "Software"), to deal
-- in the Software without restriction, including without limitation the rights
-- to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
-- copies of the Software, and to permit persons to whom the Software is
-- furnished to do so, subject to the following conditions:
--
-- The above copyright notice and this permission notice shall be included in all
-- copies or substantial portions of the Software.
--
-- THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
-- IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
-- FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
-- AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
-- LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
-- OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
-- SOFTWARE.

-- O.S library expansion

local os = require("os")
local io = require("io")

---@class ospplib: oslib
local ospp = {}
setmetatable(ospp, {__index = os})

---@return "win"|"unix"
function ospp.get()
    return package.config:sub(1,1) == "\\" and "win" or "unix"
end

---@type "nul"|"/dev/null"
ospp.devnull = (ospp.get() == "win") and "nul" or "/dev/null"

local join_paths, adjust_path

if ospp.get() == "win" then
    function join_paths(...)
        return table.concat({...}, "\\"):gsub("/", "\\"):gsub("\\+", "\\")
    end

    function adjust_path(path)
        path = path:gsub("/", "\\")
        return path
    end
else
    function join_paths(...)
        return table.concat({...}, "/"):gsub("\\", "/"):gsub("/+", "/")
    end

    function adjust_path(path)
        path = path:gsub("\\", "/")
        return path
    end
end

do
    local get_path_cmd = ospp.get() == "win" and "cd" or "pwd"

    ---@return string | nil
    function ospp.curdir()
        local cmd = io.popen(get_path_cmd)
        if not cmd then
            return nil
        end

        local path = cmd:read()
        cmd:close()

        return path
    end
end

do
    local copy_cmd = ospp.get() == "win" and "copy" or "cp -r"
    function ospp.copy(src, dst)
        src = adjust_path(src)
        dst = adjust_path(dst)

        io.popen(copy_cmd .. ' "' .. src .. '" "' .. dst .. '"')
    end
end

do
    local disk = ospp.get() == "win" and "C:\\" or "/"
    function ospp.move(path, to)
        local new_path
        if path:sub(1, #disk) == disk then
            new_path = adjust_path(to)
        else
            new_path = join_paths(to, path)
        end

        os.rename(path, adjust_path(new_path))

        return new_path
    end
end

function ospp.mkdir(dirname)
    os.execute('mkdir "' .. dirname .. '"')
end

if ospp.get() == "win" then
    function ospp.exists(path)
        path = path:gsub("/", "\\"):gsub("\\+", "\\")
        return io.popen('if exist "'..path..'" (echo ok)'):read() == "ok"
    end

    function ospp.listdir(dirpath)
        if dirpath == nil then dirpath = "." end
        dirpath = dirpath:gsub("/", "\\"):gsub("\\+", "\\")

        if not ospp.exists(dirpath) then
            return nil
        end

        local files = {}
        for file in io.popen('dir "' .. dirpath .. '" /b /a'):lines() do
            table.insert(files, file)
        end

        return files
    end

    function ospp.setenv(name, value)
        io.popen('setx ' .. name .. ' "' .. value .. '"'):close()
    end

    function ospp.rmdir(path)
        path = path:gsub("/", "\\"):gsub("\\+", "\\")
        if not ospp.exists(path) then
            return false
        end
        io.popen('rmdir /s /q "'..path..'"')
        return true
    end
else -- unix
    function ospp.exists(path)
        path = path:gsub("\\", "/"):gsub("/+", "/")
        return io.popen('[ -e "' .. path .. '" ] && echo ok'):read() == "ok"
    end

    function ospp.listdir(dirpath)
        if dirpath == nil then dirpath = "." end
        dirpath = dirpath:gsub("\\", "/"):gsub("/+", "/")

        if not ospp.exists(dirpath) then
            return nil
        end

        local p = io.popen('ls -A1 "'..dirpath..'"')
        local files = {}

        -- Read the output line by line
        for file in p:lines() do
            table.insert(files, file)
        end

        p:close()
        return files
    end

    function ospp.setenv(name, value)
        io.popen('export ' .. name .. '="' .. value .. '"')
    end

    function ospp.rmdir(path)
        path = path:gsub("\\", "/"):gsub("/+", "/")
        if not ospp.exists(path) then
            return false
        end
        io.popen('rm -rf "' .. path .. '"')
        return true
    end
end

function ospp.isfile(path)
    local f = io.open(path, "r")

    return f and (io.type(f) == "file" or io.type(f) == "closed file")
end

return ospp

