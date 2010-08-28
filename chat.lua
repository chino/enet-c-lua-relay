#!/usr/bin/env lua
package.cpath = "./lua/?/?.so;" .. package.cpath
require 'sleep'
require 'io_ready'
require 'net'

function die( str ) print(str); os.exit(1) end

ip, port = arg[1], arg[2]

if ip and port then
	hosting = false
	if not net.join( ip, port ) then
		die("failed to join")
	end
	print("connecting to ip")
	while net.state() == "connecting" do
		net.pump(nil)
		print("still connecting")
		msleep(100)
	end
	if net.state() ~= "connected" then
		die("failed to connect to ip")
	end
	print("connected")
else
	hosting = true
	if not net.host( 2300 ) then
		die("failed to host")
	end
	print("waiting for connections")
end

CHAT_CHANNEL=0
function send_msg(line) net.send( line, #line, net.flags.reliable, CHAT_CHANNEL ) end

while true do
	if io.ready(io.input()) then
		send_msg( io.read() )
	end
	net.pump(function( line )
		print("> "..line)
		if hosting then
			send_msg( line )
		end
	end)
	msleep(100)
end
