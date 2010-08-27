#!./lua/lua

package.cpath = "./lua/?/?.so;" .. package.cpath

require 'sleep'

function die( str )
	print(str)
	os.exit(1)
end

function send_text( line )
	chat_channel = 0
	network_send( line, #line, network_flags.reliable, chat_channel )
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
		msleep(100)
	end
	if not network_state() == "connected" then
		die("failed to connect to host")
	end
	print("connected")
end

-- command line arguments
ip, port = ARGV[2], ARGV[3]

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
	if io.ready() then
		send_text( io.read() )
	end
	network_pump(function( line )
		print("> "..line)
		if hosting then
			send_text( line ) 
		end
	end)
	msleep(100)
end
