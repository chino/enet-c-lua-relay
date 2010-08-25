#include "lua_common.h"

int lua_set_argv( lua_State* state, int argc, char ** argv )
{
	int i;
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

int lua_dofile( lua_State* state, char * file )
{
	int error = luaL_dofile(state,file);
	if(error)
	{
		const char * ptr = lua_tostring(state,-1);
		printf("lua error dofile: %s\n",
			ptr ? ptr : "unknown error" );
	}
	return error;
}
