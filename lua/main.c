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

static int lua_set_args( int argc, char ** argv )
{
	int i, top;
	lua_newtable(L1);
	top = lua_gettop(L1);
	for( i = 0; i < argc; i++ )
	{
		lua_pushinteger(L1,i);
		lua_pushstring(L1,argv[i]);
		lua_settable(L1,top);
	}
	lua_setglobal(L1,"args");
	return 0;
}

int main( int argc, char ** argv )
{
	if(argc < 2)
	{
		printf("Usage: %s <script>\n",argv[0]);
		return 1;
	}
	ASSERT(lua_init());
	ASSERT(lua_set_args(argc,argv));
	ASSERT(lua_register_net_proxy());
	ASSERT(lua_dofile(argv[1]));
	return 0;
}
