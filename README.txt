author:

	Daniel Aquino aka methods

contact:

	fskn.methods@gmail.com

description:

	Small example of a network server/client using an enet abstraction and lua bindings.

clone:

	bzr clone http://fly.thruhere.net/projects/net_proxy

build:

	make

clean:

	make clean

host:

	./c/chat
	./lua/chat.lua

join:

	./c/chat <host> <port>
	./lua/chat.lua <host> <port>
