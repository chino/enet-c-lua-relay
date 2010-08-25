#include "lua_common.h"
#include "lua_net_proxy.h"

static int lua_set_args( lua_State* state, int argc, char ** argv )
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

int main( int argc, char ** argv )
{
	if(argc < 2)
	{
		printf("Usage: %s <script>\n",argv[0]);
		return 1;
	}
	ASSERT(lua_init());
	ASSERT(lua_set_args(L1,argc,argv));
	ASSERT(lua_register_net_proxy());
	ASSERT(lua_dofile(argv[1]));
	return 0;
}
