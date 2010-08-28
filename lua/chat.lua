#!/usr/bin/env lua

package.cpath = "./lua/?/?.so;./?/?.so;" .. package.cpath

require 'sleep'
require 'io_ready'
require 'net'

function die( str )
	print(str)
	os.exit(1)
end

function send_text( line )
	chat_channel = 0
	net.send( line, #line, net.flags.reliable, chat_channel )
end

function host()
	hosting = true
	if not net.host( 2300 ) then
		die("failed to host")
	end
	print("waiting for connections")
end

function join( host, port )
	if not net.join( host, port ) then
		die("failed to join")
	end
	print("connecting to host")
	while net.state() == "connecting" do
		net.pump(nil)
		print("still connecting")
		msleep(100)
	end
	if not net.state() == "connected" then
		die("failed to connect to host")
	end
	print("connected")
end

-- command line arguments
ip, port = arg[1], arg[2]

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
	if io.ready(io.input()) then
		send_text( io.read() )
	end
	net.pump(function( line )
		print("> "..line)
		if hosting then
			send_text( line ) 
		end
	end)
	msleep(100)
end
