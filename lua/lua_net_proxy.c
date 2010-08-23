/* vim:set sw=4 ts=4:
 *
 * Copyright (C) 2010 Daniel Aquino
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "lua_common.h"
#include "lua_net_proxy.h"
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

static void handle_packet( network_packet_t* packet )
{
	int error = 0;
	lua_pushvalue(L1,lua_gettop(L1)); // dupe callback in case multiple packets
	lua_pushlstring(L1,(const char *)packet->data,packet->size);
	error = lua_pcall(L1,1,0,0);
	if(error)
	{
		const char * ptr = lua_tostring(L1,-1);
		printf("lua error handle_packet: %s\n",
			ptr ? (char*)ptr : "unknown error" );
	}
}

static int lua_network_pump(lua_State *state)
{	
	network_pump( handle_packet );
	lua_settop(state,0);
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

void lua_register_network_flags(void)
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
