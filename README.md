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

You can create a Ndarray with the function `ln.array`, passing as argument a table
```lua
local ln = require "luandarray"

local arr_1D = ln.array({1,2,3})
local arr_2D = ln.array({{1,2,3}, {4,5,6}})
local arr_3D = ln.array({{{1,2,3}, {4,5,6}}, {{7,8,9}, {10,11,12}}})
-- ...

You can see more about LuaNdarray in the folder `docs`
