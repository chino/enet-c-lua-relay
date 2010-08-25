#ifndef LUA_BINDINGS_H
#define LUA_BINDINGS_H

#include "lua_headers.h"

int lua_usleep( lua_State *state );
int lua_stdin_ready( lua_State *state );

#endif // LUA_BINDINGS_H
