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

lua/main.c -> lua/lua:

	example of a c program which embeds lua
	note that the stand alone lua interpreter is exactly the same thing
	supports running a single file as the first argument
	thus you can also do:
		./lua/lua ./lua/chat.lua
