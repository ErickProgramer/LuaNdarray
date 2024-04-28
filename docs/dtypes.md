# dtypes

## what are?
dtypes are specific data types such as int8, int16 etc. With them we can create integers for example of any size up to 64 bits for example an 8 bit integer would take up less memory than an integer
16, 32, 64 bits. But it would have less precision than these, an int8 varies from `-128` to `127` while an int64 varies from `-9,223,372,036,854,775,808` to `9,223,372,036,854,775,807` actually much more than an int8. On the other hand it takes up more memory than an int8. Use dtypes wisely!

- `int8`: signed integer of 8 bits
- `int16`: signed integer of 16 bits
- `int32`: signed integer of 32 bits
- `int64`: signed integer of 64 bits
- `uint8`: signed integer of 8 bits
- `uint16`: unsigned integer of 16 bits
- `uint32`: unsigned integer of 32 bits
- `uint64`: unsigned integer of 64 bits
- `float32`: float of 32 bits
- `float64`: float of 32 bits
- `complex64`: complex number of 64 bits
- `complex128`: complex number of 128 bits
- `char`: single character (8 bits)
- `bool`: 1 (`true`) or 0 (`false`) (8 bits)
- `byte`: alias for `int8`

wow, really a lot! 

# signed integers
LuaNdarray, as we saw, has integers from 8 bits to 64 bits, now we will see an example of how to create arrays of this type:

```lua
> ln = require "luandarray"
> ln.int8({1,2}) -- Note that we can also create arrays of different types by calling the types as functions
array({1    2}, dtype=int8)
> ln.array(200, ln.int8)
array(-56, dtype=int8)
```
The rule is the same for all other integers.

# unsigned integers
unsigned integers work like conventional integers, but they cannot be negative, so they support positive numbers much larger than signed integers, for example an 8-bit integer as we saw, goes from `-127` to `128`, and an unsigned integer 8-bit signal goes from `0` to `255`. They can be useful when we want large positive integers and don't need negative numbers. e.g.:
```lua
> ln = require "luandarray"
> ln.uint8({-5,-100}) -- Note that we can also create arrays of different types by calling the types as functions
array({251      246}, dtype=uint8)
> ln.array(200, ln.uint8)
array(200, dtype=uint8)
```
see that this time, the number 200 worked correctly, but when we used -5 and -100 the results were unpredictable.

# floats
floating point numbers are like Lua numbers, which may or may not have numbers after the comma, LuaNdarray has 2 types of floating numbers: float32 and float64, with float64 having twice the precision but at the same time being twice as heavy, e.g.:
```lua
> ln = require "luandarray"
> ln.float32({10.6055543, -10.09392}) -- Note that we can also create arrays of different types by calling the types as functions
array({10.605554580688  -10.093919754028}, dtype=float32)
> ln.array({10.6055543, -10.09392}, ln.float64)
array({10.6055543       -10.09392}, dtype=float64)
```
note that in float32 there was a precision problem, while in float64 the result was more accurate.

# complex
Complex numbers are numbers that have a real and imaginary part, in LuaNdarray there are 2 types of complex numbers: complex64 where the real and imaginary part are 32-bit floats and complex128 where the real and imaginary part are 64-bit floats. E.g.:

```lua
> ln = require "luandarray"
> ln.array({10+4*I, 1*I, I, -I, -100.54+1.54*I, 10}, ln.complex64)
array({10+4i    i       i       -i      -100.5+1.54i    10}, dtype=complex64)
> ln.array({10+4*I, 1*I, I, -I, -100.54+1.54*I, 10}, ln.complex128)
array({10+4i    i       i       -i      -100.5+1.54i    10}, dtype=complex128)
```

Note: `I` is a global LuaNdarray variable that is represented by a complex number with the real part being 0 and the imaginary part being 1.

# others
Now let's talk about LuaNdarray's boolean and char types, booleans can represent `true` or `false` but LuaNdarray does not store them as 1 bit in memory, instead booleans occupy 8 bits (1 byte) in memory just like int8 . And the char type, which also occupies 8 bits but is treated as a character instead of a number. There is also the byte type, which is just a different name for `int8`.

```lua
> ln = require "luandarray"
> ln.array({'a', 'b', 'c'}, ln.char) -- or ln.char({'a', 'b', 'c'})
array({'a'      'b'     'c'}, dtype=char)
> ln.array({1, 0, true, false}, ln.bool) -- or ln.bool({1, 0, true, false})
array({true     false   true    false}, dtype=bool)
> ln.array({1, 43, 127, -128}, ln.byte) -- or ln.byte({1, 43, 127, -128})
array({1        43      127     -128}, dtype=int8)
```

# IMPORTANT!
for 64-bit integers, there may be a precision problem when converting the Lua number to an int64 or uint64, this is because int64/uint64 support integers larger than Lua itself, a solution is to avoid passing too large numbers when create an array with `ln.array` or any other constructor.
