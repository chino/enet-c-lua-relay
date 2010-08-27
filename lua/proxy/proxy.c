#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include "net_proxy.h"

int l_network_host(lua_State *L)
{
	lua_pushboolean(L,
		NETWORK_OK == network_host(
			luaL_checkint(L,-1) // port
		)
	);
	return 1;
}

int l_network_join(lua_State *L)
{
	lua_pushboolean(L,
		NETWORK_OK == network_join(
			(char*) luaL_checkstring(L,-2), // host
			        luaL_checkint(L,-1)     // port
		)
	);
	return 1;
}

int l_network_send(lua_State *L)
{
	network_send(
		(char*) luaL_checkstring(L,-4), // data
		        luaL_checkint(L,-3),    // size
		        luaL_checkint(L,-2),    // flags
		        luaL_checkint(L,-1)     // channel
	);
	return 0;
}

int handle_packet( network_packet_t* packet, void* context )
{
	lua_State *L = context;
	lua_pushvalue(L,-1);
	lua_pushlstring(L,(const char *)packet->data,packet->size);
	lua_call(L,1,1);
	return lua_tointeger(L,-1);
}

int l_network_pump(lua_State *L)
{	
	network_pump( handle_packet, L );
	return 0;
}

int l_network_state(lua_State *L)
{
	switch( network_state )
	{
	case NETWORK_CONNECTED:
		lua_pushstring(L,"connected");
		break;
	case NETWORK_CONNECTING:
		lua_pushstring(L,"connecting");
		break;
	case NETWORK_DISCONNECTED:
		lua_pushstring(L,"disconnected");
		break;
	default:
		lua_pushstring(L,"unknown");
	}
	return 1;
}

int l_network_quit(lua_State *L)
{
	network_quit();
	return 0;
}

void l_register_network_flags(lua_State *L)
{
	lua_newtable(L);

	lua_pushstring(L,"reliable");
	lua_pushnumber(L,NETWORK_RELIABLE);
	lua_settable(L,-3);

	lua_pushstring(L,"sequenced");
	lua_pushnumber(L,NETWORK_SEQUENCED);
	lua_settable(L,-3);

	lua_setglobal(L,"network_flags");
}

int luaopen_proxy(lua_State *L)
{
	lua_register(L,"network_host",l_network_host);
	lua_register(L,"network_join",l_network_join);
	lua_register(L,"network_send",l_network_send);
	lua_register(L,"network_pump",l_network_pump);
	lua_register(L,"network_quit",l_network_quit);
	lua_register(L,"network_state",l_network_state);
	l_register_network_flags(L);
	return 1;
}
