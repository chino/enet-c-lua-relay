#include "lua_common.h"
#include <unistd.h>

int lua_usleep( lua_State *state )
{
	usleep( luaL_checkint(state,-1) );
	return 0;
}

int lua_stdin_ready( lua_State *state )
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
