#include "lua_common.h"
#include "lua_bindings.h"
#include "lua_net_proxy.h"

static void register_io_ready( lua_State* state )
{
	lua_getglobal(state,"io");
	lua_pushstring(state,"ready");
	lua_pushcfunction(state,lua_stdin_ready);
	lua_settable(state,1);
}

static void lua_register_functions( lua_State* state )
{
	register_io_ready(state);
	lua_register( state, "usleep", lua_usleep );
}

int main( int argc, char ** argv )
{
	lua_State *L1;

	if ( argc < 2 )
	{
		printf( "Usage: %s <script>", argv[0] );
		return 1;
	}

	if ( ! (L1 = luaL_newstate()) )
	{
		printf("lua: error initializing %s\n",
			lua_tostring(L1,-1));
		return 1;
	}

	luaL_openlibs(L1);
	lua_register_functions( L1 );

	if ( lua_set_argv( L1, argc, argv ) )
	{
		puts( "lua: failed to set ARGV" );
		goto cleanup;
	}

	if ( lua_register_net_proxy( L1 ) )
	{
		puts( "lua: failed to register net proxy" );
		goto cleanup;
	}

	if( lua_dofile( L1, argv[1] ) )
	{
		printf( "lua: failed to dofile '%s'; %s\n",
			argv[1], lua_tostring( L1, -1 ));
	}

cleanup:

	if(L1)
		lua_close(L1);

	return 0;
}
