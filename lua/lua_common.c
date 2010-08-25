#include "lua_common.h"
#include <unistd.h>

lua_State *L1;

int lua_set_argv( lua_State* state, int argc, char ** argv )
{
	int i, table;
	lua_newtable(state);
	for( i = 0; i < argc; i++ )
	{
		lua_pushnumber(state,i);
		lua_pushstring(state,argv[i]);
		lua_settable(state,-3);
	}
	lua_setglobal(state,"ARGV");
	return 0;
}

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
	usleep( luaL_checkint(state,-1) );
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
