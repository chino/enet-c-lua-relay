#include "lua_common.h"
#include "lua_net_proxy.h"

static int lua_set_args( int argc, char ** argv )
{
	int i, table;
	lua_newtable(L1);
	table = lua_gettop(L1);
	for( i = 0; i < argc; i++ )
	{
		lua_pushnumber(L1,i);
		lua_pushstring(L1,argv[i]);
		lua_settable(L1,table);
	}
	lua_setglobal(L1,"ARGV");
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
	ASSERT(lua_set_args(argc,argv));
	ASSERT(lua_register_net_proxy());
	ASSERT(lua_dofile(argv[1]));
	return 0;
}
