#include "lua_headers.h"
#include "net_proxy.h"

static int lua_network_host(lua_State *state)
{
	lua_pushboolean(state,
		NETWORK_OK == network_host(
			luaL_checkint(state,-1) // port
		)
	);
	return 1;
}

static int lua_network_join(lua_State *state)
{
	lua_pushboolean(state,
		NETWORK_OK == network_join(
			(char*) luaL_checkstring(state,-2), // host
			        luaL_checkint(state,-1)     // port
		)
	);
	return 1;
}

static int lua_network_send(lua_State *state)
{
	network_send(
		(char*) luaL_checkstring(state,-4), // data
		        luaL_checkint(state,-3),    // size
		        luaL_checkint(state,-2),    // flags
		        luaL_checkint(state,-1)     // channel
	);
	return 0;
}

static int handle_packet( network_packet_t* packet, void* context )
{
	lua_State *state = context;
	lua_pushvalue(state,-1); // for next iteration
	lua_pushlstring(state,(const char *)packet->data,packet->size);
	if( ! lua_pcall(state,1,1,0) )
		return lua_tointeger(state,-1);
	printf("lua: error in handle_packet %s\n",
		lua_tostring(state,-1) );
	return 0;
}

static int lua_network_pump(lua_State *state)
{	
	network_pump( handle_packet, state );
	return 0;
}

static int lua_network_state(lua_State *state)
{
	switch( network_state )
	{
	case NETWORK_CONNECTED:
		lua_pushstring(state,"connected");
		break;
	case NETWORK_CONNECTING:
		lua_pushstring(state,"connecting");
		break;
	case NETWORK_DISCONNECTED:
		lua_pushstring(state,"disconnected");
		break;
	default:
		lua_pushstring(state,"unknown");
	}
	return 1;
}

static int lua_network_quit(lua_State *state)
{
	network_quit();
	return 0;
}

static void lua_register_network_flags(lua_State *state)
{
	lua_newtable(state);

	lua_pushstring(state,"reliable");
	lua_pushnumber(state,NETWORK_RELIABLE);
	lua_settable(state,-3);

	lua_pushstring(state,"sequenced");
	lua_pushnumber(state,NETWORK_SEQUENCED);
	lua_settable(state,-3);

	lua_setglobal(state,"network_flags");
}

int lua_register_net_proxy(lua_State *state)
{
	lua_register(state,"network_host",lua_network_host);
	lua_register(state,"network_join",lua_network_join);
	lua_register(state,"network_send",lua_network_send);
	lua_register(state,"network_pump",lua_network_pump);
	lua_register(state,"network_quit",lua_network_quit);
	lua_register(state,"network_state",lua_network_state);
	lua_register_network_flags(state);
	return 0;
}
