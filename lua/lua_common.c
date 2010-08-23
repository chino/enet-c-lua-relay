/* vim:set sw=4 ts=4:
 *
 * Copyright (C) 2010 Daniel Aquino
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "lua_common.h"
#include <unistd.h>

lua_State *L1;

int lua_dofile( char * file )
{
	int error = luaL_dofile(L1,file);
	if(error)
	{
		const char * ptr = lua_tostring(L1,-1);
		printf("lua error dofile: %s\n",
			ptr ? ptr : "unknown error" );
	}
	return error;
}

static int lua_create(void)
{
	L1 = luaL_newstate();
	if (!L1)
	{
		puts("lua: error initializing");
		return 1;
	}
	luaL_openlibs(L1);
	return 0;
}

void lua_shutdown(void)
{
	if(L1)
		lua_close(L1);
}

static int lua_usleep(lua_State *state)
{
	int time = luaL_checkint(state,lua_gettop(state));
	usleep(time);
	return 0;
}

static int lua_stdin_ready(lua_State *state)
{
	int rv;
	struct timeval tv;
	fd_set readfds;

	tv.tv_sec  = 0;
	tv.tv_usec = 1; // microsecond

	FD_ZERO(&readfds);
	FD_SET(fileno(stdin), &readfds);

	rv = select( fileno(stdin)+1, &readfds, NULL, NULL, &tv );
	lua_pushboolean(state, rv > 0);
	return 1;
}

void lua_register_stdin_ready(void)
{
	lua_getglobal(L1,"io");
	lua_pushstring(L1,"ready");
	lua_pushcfunction(L1,lua_stdin_ready);
	lua_settable(L1,1);
}

int lua_register_functions(void)
{
	lua_register(L1,"usleep",lua_usleep);
	lua_register_stdin_ready();
	return 0;
}

int lua_init(void)
{
	atexit(lua_shutdown);
	ASSERT(lua_create());
	ASSERT(lua_register_functions());
	return 0;
}
