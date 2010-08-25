#include "lua_common.h"
#include "net_proxy.h"

static int lua_network_host(lua_State *state)
{
	int port = luaL_checkint(state,1);
	int rv = network_host(port);
	lua_pushboolean(state,rv==NETWORK_OK);
	return 1;
}

static int lua_network_join(lua_State *state)
{
	char * host = (char*) lua_tostring(state,1);
	int port = luaL_checkint(state,2);
	int rv = network_join(host,port);
	lua_pushboolean(state,rv==NETWORK_OK);
	return 1;
}

static int lua_network_send(lua_State *state)
{
	char * data = (char*) lua_tostring(state,1);
	int size = luaL_checkint(state,2);
	int flags = luaL_checkint(state,3);
	int channel = luaL_checkint(state,4);
	network_send(data,size,flags,channel);
	return 0;
}

static void handle_packet( network_packet_t* packet, void* context )
{
	lua_State *state = context;
	int error = 0;
	lua_pushvalue(state,-1); // because pcall will remove it
	lua_pushlstring(state,(const char *)packet->data,packet->size);
	error = lua_pcall(state,1,0,0);
	if(error)
	{
		const char * ptr = lua_tostring(state,-1);
		printf("lua error handle_packet: %s\n",
			ptr ? (char*)ptr : "unknown error" );
	}
	lua_pop(state,1);
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

static void lua_register_network_flags(void)
{
	int top;
	lua_newtable(L1);
	top = lua_gettop(L1);

	lua_pushstring(L1,"reliable");
	lua_pushinteger(L1,NETWORK_RELIABLE);
	lua_settable(L1,top);

	lua_pushstring(L1,"sequenced");
	lua_pushinteger(L1,NETWORK_SEQUENCED);
	lua_settable(L1,top);

	lua_setglobal(L1,"network_flags");
}

int lua_register_net_proxy( void )
{
	lua_register(L1,"network_host",lua_network_host);
	lua_register(L1,"network_join",lua_network_join);
	lua_register(L1,"network_send",lua_network_send);
	lua_register(L1,"network_pump",lua_network_pump);
	lua_register(L1,"network_quit",lua_network_quit);
	lua_register(L1,"network_state",lua_network_state);
	lua_register_network_flags();
	return 0;
}
