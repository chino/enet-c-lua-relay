#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

static void l_set_arg( lua_State* state, int argc, char ** argv )
{
	int i;
	lua_newtable(state);
	for( i = 0; i < argc; i++ )
	{
		lua_pushnumber(state,i-1);
		lua_pushstring(state,argv[i]);
		lua_settable(state,-3);
	}
	lua_setglobal(state,"arg");
	return 0;
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

	l_set_arg( L1, argc, argv );

	if( luaL_dofile( L1, argv[1] ) )
	{
		printf( "lua: failed to dofile '%s'; %s\n",
			argv[1], lua_tostring(L1,-1));
	}

	if(L1)
		lua_close(L1);

	return 0;
}
