#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <stdlib.h>

int l_io_ready(lua_State *L)
{
	int rv = 0, fn;
	struct timeval tv;
	fd_set readfds;

	fn = fileno(stdin); // ((FILE*)lua_touserdata(L,-1));
	if(fn == -1)
		goto finished;
		
	tv.tv_sec  = 0;
	tv.tv_usec = 1; // microseconds

	FD_ZERO(&readfds);
	FD_SET(fn, &readfds);

	rv = select( fn+1, &readfds, NULL, NULL, &tv );

finished:

	lua_pushboolean(L, rv > 0);
	return 1;
}

int luaopen_io_ready(lua_State *L)
{
	lua_getglobal(L,"io");
	lua_pushstring(L,"ready");
	lua_pushcfunction(L,l_io_ready);
	lua_settable(L,-3);
	return 1;
}
