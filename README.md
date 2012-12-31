Description:

	A relay server.

	Data sent by a connection will be mirrored to other connections.

	Server and clients provided in C and Lua.

Why?

	This serves mainly as an example.

	It demonstrates using Enet, binding C to Lua, and a simple relay server.

Build:

	make

Server:


	./chat
	./chat.lua

Client:

	./chat <host> <port>
	./chat.lua <host> <port>
