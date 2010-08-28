#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <unistd.h>

static int l_usleep(lua_State *L)
{
	usleep( luaL_checkint(L,-1) );
	return 0;
}

static int l_msleep(lua_State *L)
{
	usleep( luaL_checkint(L,-1) * 1000 );
	return 0;
}

static int l_sleep(lua_State *L)
{
	sleep( luaL_checkint(L,-1) );
	return 0;
}

int luaopen_sleep(lua_State *L)
{
	lua_register( L, "usleep", l_usleep );
	lua_register( L, "msleep", l_msleep );
	lua_register( L, "sleep",  l_sleep  );
	return 1;
}
