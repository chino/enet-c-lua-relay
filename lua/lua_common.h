#ifndef LUA_COMMON_H
#define LUA_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

int lua_dofile   ( lua_State*, char* );
int lua_set_argv ( lua_State*, int argc, char ** argv );

#endif // LUA_COMMON_H
