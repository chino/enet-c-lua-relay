#!./lua/lua
--[[
 vim:set sw=4 ts=4:
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
--]]

function die( str )
	print(str)
	os.exit(1)
end

function send_text( line )
	chat_channel = 0
	network_send( line, #line, network_flags.reliable, chat_channel )
end

function check_input()
	if io.ready() then
		line = io.read()
		send_text( line )
	end
end

function host()
	hosting = true
	if not network_host( 2300 ) then
		die("failed to host")
	end
	print("waiting for connections")
end

function join( host, port )
	if not network_join( host, port ) then
		die("failed to join")
	end
	print("connecting to host")
	while network_state() == "connecting" do
		network_pump(nil)
		print("still connecting")
		usleep(1000*100)
	end
	if not network_state() == "connected" then
		die("failed to connect to host")
	end
	print("connected")
end

-- command line arguments
ip, port = args[2], args[3]

-- host or join a host
hosting = false
if ip and port then
	join( ip, port )
else
	hosting = true
	host()
end

-- main loop
while true do
	check_input()
	network_pump(function( line )
		print("> "..line)
		if hosting then
			send_text( line ) 
		end
	end)
	usleep(1000*100) -- 100ms
end
