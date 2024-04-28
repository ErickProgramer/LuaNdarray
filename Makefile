ifeq ($(OS), Windows_NT)
	COPYCMD := copy
else
	COPYCMD := cp
endif

ifeq ($(LIB_EXTENSION), dll)
	EXEC_EXT := exe
else
	EXEC_EXT := out
endif

CODE := 'local res = string.gsub(string.gsub(string.lower(_VERSION), "[ .]", ""), "lua", "") print(res)'
LUA_VERSION = $(shell $(LUA) -e $(CODE))
LUA_LIB = -llua$(LUA_VERSION)

ARCH := $(shell $(CC) detect_simd.c -o detect_simd.$(EXEC_EXT))
ARCH := -m$(shell ./detect_simd)

ifeq (ARCH, -m)
	ARCH :=
endif

all:
	$(CC) $(CFLAGS) $(ARCH) -march=native -shared -I$(LUA_INCDIR) -L$(LUA_LIBDIR) multiarray/*.c multiarray/simd/*.c -o core.$(LIB_EXTENSION) $(LUA_LIB)

install:
	$(COPYCMD) core.$(LIB_EXTENSION) "$(ENV_INST_PREFIX)"
