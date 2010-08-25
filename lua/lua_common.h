#ifndef LUA_COMMON_H
#define LUA_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#define ASSERT(x) \
        { \
                int _err = x; \
                if (_err) return _err; \
        }

extern lua_State *L1;

int lua_dofile(char*);
int lua_init(void);
void lua_shutdown(void);
int lua_set_argv( lua_State* state, int argc, char ** argv );

#endif /* LUA_COMMON_H */
