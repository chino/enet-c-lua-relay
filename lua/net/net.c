#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include "net.h"

static int l_network_host(lua_State *L)
{
	lua_pushboolean(L,
		NETWORK_OK == network_host(
			luaL_checkint(L,-1) // port
		)
	);
	return 1;
}

static int l_network_join(lua_State *L)
{
	lua_pushboolean(L,
		NETWORK_OK == network_join(
			(char*) luaL_checkstring(L,-2), // host
			        luaL_checkint(L,-1)     // port
		)
	);
	return 1;
}

static int l_network_send(lua_State *L)
{
	network_send(
		(char*) luaL_checkstring(L,-4), // data
		        luaL_checkint(L,-3),    // size
		        luaL_checkint(L,-2),    // flags
		        luaL_checkint(L,-1)     // channel
	);
	return 0;
}

static int handle_packet( network_packet_t* packet, void* context )
{
	lua_State *L = context;
	lua_pushvalue(L,-1);
	lua_pushlstring(L,(const char *)packet->data,packet->size);
	lua_call(L,1,1);
	return lua_tointeger(L,-1);
}

static int l_network_pump(lua_State *L)
{	
	network_pump( handle_packet, L );
	return 0;
}

static int l_network_state(lua_State *L)
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

static int l_network_quit(lua_State *L)
{
	network_quit();
	return 0;
}

#define SET_NUMBER(L,name,number)\
	lua_pushstring(L,name);\
	lua_pushnumber(L,number);\
	lua_settable(L,-3);

static void create_flags(lua_State *L)
{
	lua_pushstring(L,"flags");
	lua_createtable(L,0,2);
	SET_NUMBER(L,"reliable",NETWORK_RELIABLE);
	SET_NUMBER(L,"sequenced",NETWORK_SEQUENCED);
	lua_settable(L,-3);
}

static const struct luaL_Reg net [] = {
	{"host",l_network_host},
	{"join",l_network_join},
	{"send",l_network_send},
	{"pump",l_network_pump},
	{"quit",l_network_quit},
	{"state",l_network_state},
	{NULL,NULL},
};

int luaopen_net(lua_State *L)
{
	luaL_register(L, "net", net);
	create_flags(L);
	return 1;
}