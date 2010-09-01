#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include "net.h"

#define SET_NUMBER(L,name,number)\
	lua_pushstring(L,name);\
	lua_pushnumber(L,number);\
	lua_settable(L,-3);

#define SET_STRING(L,name,string)\
	lua_pushstring(L,name);\
	lua_pushstring(L,string);\
	lua_settable(L,-3);

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
			(char*)
			luaL_checkstring(L,-2), // host
			luaL_checkint(L,-1)     // port
		)
	);
	return 1;
}

static int l_network_broadcast(lua_State *L)
{
	network_broadcast(
		(char*)
		luaL_checkstring(L,-4), // data
		luaL_checkint(L,-3),    // size
		luaL_checkint(L,-2),    // flags
		luaL_checkint(L,-1)     // channel
	);
	return 0;
}

static int l_network_send(lua_State *L)
{
	lua_getfield(L,-5,"ptr");
	network_send(
		(network_connection_t*)
		lua_touserdata(L,-1),   // connection
		(char*)
		luaL_checkstring(L,-5), // data
		luaL_checkint(L,-4),    // size
		luaL_checkint(L,-3),    // flags
		luaL_checkint(L,-2)     // channel
	);
	return 0;
}

static void push_connection( lua_State* L, network_connection_t * connection )
{
	void* data;
	lua_createtable(L,0,3);
	SET_STRING(L,"ip",connection->ip);
	SET_NUMBER(L,"port",connection->port);
	lua_pushstring(L,"ptr");
	data = lua_newuserdata(L,sizeof(connection));
	memcpy(data,connection,sizeof(connection));
	lua_settable(L,-3);
}

static int handle_packet( network_event_t type, void* data, void* context )
{
	lua_State *L = context;
	if(lua_isnil(L,-1))
		return 0;
	lua_pushvalue(L,-1);
	switch(type)
	{
	case NETWORK_EVENT_CONNECT:
	case NETWORK_EVENT_DISCONNECT:
		{
			lua_pushstring(L,
				type == NETWORK_EVENT_CONNECT ?
					"connect" : "disconnect" );
			push_connection(L,(network_connection_t*)data);
			lua_call(L,2,1);
		}
		break;
	case NETWORK_EVENT_PACKET:
		{
			network_packet_t* packet = data;
			lua_pushstring(L,"data");
			push_connection(L,packet->from);
			lua_pushlstring(L,(const char *) packet->data, packet->size);
			lua_call(L,3,1);
		}
		break;
	}
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

static int l_network_flush(lua_State *L)
{
	network_flush();
	return 0;
}

static int l_network_quit(lua_State *L)
{
	network_quit();
	return 0;
}

static void create_flags(lua_State *L)
{
	lua_pushstring(L,"flags");
	lua_createtable(L,0,2);
	SET_NUMBER(L,"reliable",  NETWORK_RELIABLE);
	SET_NUMBER(L,"unreliable",NETWORK_UNRELIABLE);
	SET_NUMBER(L,"sequenced", NETWORK_SEQUENCED);
	lua_settable(L,-3);
}

static const struct luaL_Reg net [] = {
	{"host",l_network_host},
	{"join",l_network_join},
	{"pump",l_network_pump},
	{"quit",l_network_quit},
	{"flush",l_network_flush},
	{"state",l_network_state},
	{"send",l_network_send},
	{"broadcast",l_network_broadcast},
	{NULL,NULL},
};

int luaopen_net(lua_State *L)
{
	luaL_register(L, "net", net);
	create_flags(L);
	return 1;
}
