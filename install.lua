package.path = package.path .. ";?.lua"

-- This table stores colors used by luandarray installer
local colors = {
    red = "\27[31m",
    red_bold = "\27[31;1m",

    green = "\27[32m",
    green_bold = "\27[32;1m",

    yellow = "\27[33m",
    yellow_bold = "\27[33;1m",

    blue = "\27[34m",
    blue_bold = "\27[34;1m",

    pink = "\27[35m",
    pink_bold = "\27[35;1m",

    bold = "\27[;1m",

    none = "\27[0m"
}

-- LuaNdarray's write function
local function lnwrite(txt)
    txt = tostring(txt)
    local i = 0
    while i < #txt do
        i=i + 1

        if txt:sub(i,i+1) == "\\<" then
            io.write("<")
            i=i + 1
        elseif txt:sub(i,i) == "<" then
            local color = txt:sub(i, -1):match("[<]([%w_]+)[>]")
            io.write(colors[color] or error("color '" .. color .."' does not exist"))
            i=i + #color + 1
        else
            io.write(txt:sub(i,i))
        end
    end
end

function print(...)
    local args = {...}

    for i, a in ipairs(args) do
        lnwrite(a)
        if i < #args then
            io.write("\t")
        end
    end
    io.write("\n")
end

function NOTICE(msg)
    print("<yellow>[NOTICE]<none> " .. msg)
    return 0
end

function SUCCESS(msg)
    print("<green>[SUCCESS]<none> " .. msg)
end

function ERROR(msg)
    print("<red_bold>[ERROR]<none> " .. msg)
    return 1
end

function WARNING(msg)
    print("<red>[WARNING]<none> " .. msg)
end

local os = require "ospp"

local lfs_support, lfs = pcall(require, "lfs")
if not lfs_support then
   WARNING("LuaFileSystem not detected, backing to os++")
end

-- Some useful classes
local CCmeta = {
    __tostring = function (self)
        return string.format('CC("%s")', self.name)
    end
}
CCmeta.__index = CCmeta

local function newCC(name)
    local self = setmetatable({}, CCmeta)
    self.name = name
    return self
end

function CCmeta:generateObj(source, dst, flags)
    if dst == nil then
        dst = source:sub(1, -3) .. ".o"
    end
    if flags == nil then
        flags = ""
    end

    os.execute(self.name .. " " .. flags .. " -c " .. source .. " -o " .. dst)
    return os.exists(dst), dst -- Compiled successfully?
end

function CCmeta:link(flags, outname, o_files)
    os.execute(self.name .. " " .. flags .. " -o " .. outname .. " " .. table.concat(o_files, " "))
    return os.exists(outname)
end

-- Returns true if the compiler is supported
function CCmeta:supported()
    return os.execute(self.name .. " -v > " .. os.devnull .. " 2>&1") ~= nil
end

-- for detect ospp.lua
package.path = package.path .. ";?.lua"

local function adjust_path(path)
    if package.config:sub(1,1) == "/" then
        return path:gsub("/", "\\"):gsub("\\+", "\\")
    end
    return path:gsub("\\", "/"):gsub("/+", "/")
end

local function SUCCESSFULLY()
    SUCCESS("luandarray installed successfully")
    os.exit(0)
end

local function INSTALL_ERROR()
    SUCCESS("luandarray installed successfully")
    os.exit(0)
end

local function compiler_exists(cc)
    return os.execute(cc.." -v > " .. os.devnull .. " 2>&1") ~= nil
end

local function cacheExists()
    return io.open(".cache", "r") ~= nil
end

local function createCache(cc, simd)
    local f = io.open(".cache", "w")
    if f then
        f:write("CC:"..cc .. "\nsimd:"..simd)
        f:close()
    end
end

local function getCacheInfos()
    local cache_file = io.lines(".cache")

    local infos = {}

    for line in cache_file do
        local conf_name, conf_value = line:match("(%w+)%:(.+)")
        if not conf_name then break end
        infos[conf_name] = conf_value
    end

    return infos
end

local function detect_cc()

    for i, cc in ipairs({"gcc", "mingw64-gcc", "mingw32-gcc", "clang"}) do
        if newCC(cc):supported() then
            return cc
        end
    end

    -- No compiler detected
    return nil
end

local function get_OS()
    if jit then
        return jit.os
    end
    local success, name = pcall(io.popen, "uname -s")

    if success and name then return name:read() end

    local nt = os.getenv("Windows_NT")
    if nt then
        return nt:lower():match("windows") ~= nil and "windows" or "unknows"
    end

    return "unknown"
end

-- detects simd architecture
local function detect_simd(cc)
    os.execute(cc .. " detect_simd.c -o detect_simd")

    local simd = io.popen((os.get() == "unix" and "./" or "") .. "detect_simd" .. (os.get() == "win" and ".exe" or ""), "r"):read("*a")
    return simd
end

-- Initialize LuaNdarray's folder
local function init()
    if lfs_support then
        if not lfs.attributes("luandarray") then
            lfs.mkdir("luandarray")
        end

        if not lfs.attributes("luandarray/luajit") then
            os.mkdir("luandarray/luajit")
        end

        if not lfs.attributes("luandarray/luajit/bin") then
            lfs.mkdir("luandarray/luajit/bin")
        end

        return
    end
    if not os.exists("luandarray") then
        os.mkdir("luandarray")
    end

    if not os.exists("luandarray/luajit") then
        os.mkdir("luandarray/luajit")
    end

    if not os.exists("luandarray/luajit/bin") then
        os.mkdir("luandarray/luajit/bin")
    end
end

local generateObjFiles
if lfs_support then
    function generateObjFiles(cc, flags, dir)
        local obj_files = {}        for file in lfs.dir(dir) do
            if file:sub(-2, -1) == ".c" then
                local success, out = cc:generateObj(dir .. "/" .. file, nil, flags)
                if not success then
                    ERROR("error on generate object file <bold>" .. out .. "<none>")
                    os.exit(1)
                end

                NOTICE("compiling <bold>" .. dir .. "/" .. file.."<none>")
                table.insert(obj_files, out)
            end
        end

        return obj_files
    end
else
    function generateObjFiles(cc, flags, dir)
        local files = os.listdir(dir)
        local obj_files = {}
        for i, file in ipairs(files) do
            -- only .c files will be compiled
            if file:sub(-2, -1) == ".c" then
                local success, out = cc:generateObj(dir .. "/" .. file, nil, flags)
                table.insert(obj_files, out)
                if not success then
                    ERROR("error on generate object file <bold>" .. out .. "<none>")
                    os.exit(1)
                end
                NOTICE("compiling <bold>" .. dir .. "/" .. file.."<none>")
            end
        end
        return obj_files
    end
end

-- Returns the full path where Lua interpreter is.
local function getLuaInterpreterFolder()
    local i_min = 0
    while arg[ i_min] do
        i_min = i_min - 1
    end

    local path = arg[i_min+1]
    local sep = package.config:sub(1,1)
    for i=#path, 1, -1 do
        if path:sub(i,i) == sep then
            path = path:sub(1, i-1)
            break
        end
    end

    return path
end

local function luaLibPathExistsIn(dir, version)
    version = version or _VERSION:match("[%d%.]+")
    local version_num = version:gsub("%.", "")
    local ext = os.get() == "win" and "dll" or "so"


    local finder

    if jit and os.get() == "unix" then
        function finder(name)
            return name:match("libluajit%-" .. version:gsub("%.", "%.") .. ".so%.%d+")
        end
    else
        if os.get() == "win" then
            function finder(name)
                return name == "lua" .. version .. ".dll" or
                       name == "lua" .. version_num .. ".dll"
            end
        else
            function finder(name)
                return name:match("liblua" .. version:gsub("%.", "%%.") .. "%.so%.%d+") or
                       name:match("liblua" .. version_num .. "%.so%.%d+") or
                       name == "liblua.so"
            end
        end
    end

    if lfs_support then
        for file in lfs.dir(dir) do
            if finder(file) then
                return file
            end
        end
    else
        for _, file in ipairs(os.listdir(dir)) do
            if finder(file) then
                return file
            end
        end
    end

    return nil -- not found
end

local lualib_search_in = os.get() == "win" and {getLuaInterpreterFolder(), "C:\\lua"} or {
    "/usr/lib/x86_64-linux-gnu", "/usr/local/lib", "/usr/lib32", "/usr/lib64","/usr/share",
    "/lib/x86_64-linux-gnu"
}
local function getLuaLibPath()
    local sep = package.config:sub(1,1)
    for i, folder in ipairs(lualib_search_in) do
        if lfs_support and lfs.attributes(folder) or os.exists(folder) then
            local name = luaLibPathExistsIn(folder)
            if name then
                return name, (folder .. sep .. name):gsub(sep.."+", sep)
            end
        end
    end

    return nil
end

local function removeObjFiles(dir)
    local files = os.listdir(dir)
    if not files then return nil end

    for i, file in ipairs(files) do
        if file:sub(-2, -1) == ".o" then
            os.remove(dir.."/"..file)
        end
    end

    return true
end

local function lnRemoveObjFiles()
    removeObjFiles("src/core")
    removeObjFiles("src/lua/src")
    removeObjFiles("src/core/generic")
    removeObjFiles("src/core/simd")
end

local function getFlags()
    local flags = {}
    for _, argv in ipairs(arg) do
        local name, value = argv:match("(%w+)%=(%S+)")
        if name and value then
            flags[name] = value
        end
    end
    return flags
end

-- Compiles LuaNdarray's source code
local function compile(cc, simd, lualib)
    assert(lualib, "lualib is required")
    lualib = lualib:gsub("%.so.+", ""):gsub("lib", ""):gsub("%.dll", "")
    -- stores every file
    local files = os.listdir("src/core")
    if not files then
        ERROR "error on compile LuaNdarray (core folder was not found)"
        os.exit(1)
    end

    local fpic = (os.get() == "unix" and "-fPIC" or "") .. " -m" .. simd

    local all_files = {}
    for i, fs in ipairs({
        generateObjFiles(cc,  fpic, "src/core"),
        generateObjFiles(cc, fpic .. " -Isrc/core -Isrc/lua/include", "src/lua/src"),
        generateObjFiles(cc, fpic, "src/core/generic"),
        generateObjFiles(cc, fpic, "src/core/simd")
    }) do
        for _, f in ipairs(fs) do
            table.insert(all_files, f)
        end
    end

    NOTICE("linking LuaNdarray...")
    local flags = ("-fPIC -shared ")
    for i, dir in ipairs(lualib_search_in) do
        flags=flags .. "-L" .. dir .. " "
    end
    flags=flags .. "-l"..lualib..""

    local dst = os.get() == "win" and "bin\\luandarray.dll" or "bin/luandarray.so"
    if not cc:link(flags, dst, all_files) then
        ERROR("Error on link LuaNdarray")
        os.exit(1)
    end
end

local function push_lua_files()
    local files = os.listdir("src/luajit")
    if not files then
        ERROR("luajit files not found")
        os.exit(1)
    end

    for i, file in ipairs(files) do
        os.copy("src/luajit/"..file, "luandarray/luajit/"..file)
    end
end

local function main(arg)
    NOTICE("OS detected: <green_bold>" .. get_OS() .. "<none>")
    NOTICE("Initalizing luandarray folder...")
    init()

    local flags = getFlags()

    if not flags.lualib then
        NOTICE("searching Lua library...")
        flags.lualib = getLuaLibPath()
        if flags.lualib then
            NOTICE("Lua Library detected in <bold>" .. flags.lualib .."<none>")
        else
            ERROR("Lua library was not found. Please specify the Lua path by adding the flag: --lualib=\\<path>")
            return 1
        end
    else
        NOTICE("Lua library in use: " .. flags.lualib)
    end

    local CC, simd
    
    if flags.cc then
        if not compiler_exists(flags.cc) then
            ERROR("C Compiler '" .. flags.cc.."' not found")
            return 1
        else
            NOTICE("using C compiler <green_bold>'" .. flags.cc .. "'<none>")
            CC = newCC(flags.cc)
        end
    end

    if flags.simd then
        NOTICE("using SIMD architecture <green_bold>'" .. flags.simd .. "'<none>")
        simd = flags.simd
    end

    if cacheExists() then
        local infos = getCacheInfos()
        if not CC then
            CC = newCC(infos.CC)
            NOTICE("C compiler: <green_bold>" .. CC.name .."<none> (from cache)")
        end
        if not simd then
            simd = infos.simd
            NOTICE("simd: <green_bold>"..simd.."<none> (from cache)")
        end
    else
        CC = detect_cc()
        if not CC then
            ERROR("No C Compiler detected")
            return 1
        end
        NOTICE("C Compiler detected: <green_bold>" .. CC .."<none>")

        simd = detect_simd(CC)
        NOTICE("SIMD architecture detect: <green_bold>" .. simd .. "<none>")

        createCache(CC, simd)
        CC = newCC(CC)
    end

    NOTICE("compiling LuaNdarray...")
    compile(CC, simd, flags.lualib)

    NOTICE("copying lua files...")
    push_lua_files()

    NOTICE("removing object files...")
    lnRemoveObjFiles()

    return 0
end

local status = main(arg)
if status == 1 then
    INSTALL_ERROR()
else
    SUCCESSFULLY()
end
