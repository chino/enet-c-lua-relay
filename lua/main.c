#include "lua_common.h"
#include "lua_net_proxy.h"

int main( int argc, char ** argv )
{
	if(argc < 2)
	{
		printf("Usage: %s <script>\n",argv[0]);
		return 1;
	}
	ASSERT(lua_init());
	ASSERT(lua_set_argv(L1,argc,argv));
	ASSERT(lua_register_net_proxy());
	ASSERT(lua_dofile(argv[1]));
	return 0;
}
