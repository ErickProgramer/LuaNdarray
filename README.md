# LuaNdarray

LuaNdarray is a high-performance linear algebra library for Lua, implemented in C for maximum performance.

## Data Types

LuaNdarray currently primarily supports `double` data types. While we plan to expand support for various data types in future releases, please be aware that as of now, LuaNdarray does not have full support for different data types.

We are actively working on improving and expanding the capabilities of LuaNdarray, and your feedback and contributions are highly appreciated.


## Installation

To install LuaNdarray just use the command:
```
luarocks install luandarray
```
easy, right?

## LuaNdarray Overview

### Creating a Ndarray
LuaNdarray provides a low-level function for creating an Ndarray, which requires passing the data as a vector and its shape:
```lua
local ln = require "luandarray"

local data = {
    1,2,3,4,5,
    6,7,8,9,10,
    11,12,13,14,15
}
local myarr = ln.newNdarray(data, {3,5})
print(myarr)
```
However, while we don't have a higher-level `ln.array` function, you can therefore in your code have it:
```lua
local ln = require "luandarray"
function ln.array(data)
    local flatten = {}
    local function get_flatten(v)
        if type(v) ~= "table" then
            table.insert(flatten, v)
            return
        end

        for i = 1, #v do
            get_flatten(v[i])
        end
    end
    get_flatten(data)

    local function get_shape(v)
        local shape = {}
        while type(v) == "table" do
            table.insert(shape, #v)
            v = v[1]
        end
        return shape
    end
    local shape = get_shape(data)

    return ln.newNdarray(flatten, shape)
end

local arr = ln.array({{1,2,3,4,5},
                      {6,7,8,9,10}})
print(arr)
```
other functions:
```lua
local ln = require "luandarray"

print(ln.full(5, {2,2}))
print(ln.zeros({2,2}))
print(ln.ones({2,2}))
```

### Indexing a Ndarray
```lua
local ln = require "luandarray"
ln.IndexStartZero(true)

local arr = ln.zeros({3,2,4})
print(arr[0])
print(arr[1])

ln.IndexStartZero(false)
print(arr[1])
print(arr[2])
print(arr[0]) -- error
```
If the `ln.Index Start Zero` function is not called, by default it starts at `1`, but it is always good to call it with whatever value you want.
