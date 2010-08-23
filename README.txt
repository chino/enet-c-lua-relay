Small example of a network server/client using an enet abstraction and lua bindings.

To build:

	cd c; make
	cd ..
	cd lua; make
	cd ..

To host:

  ./chat
  ./chat.lua

To join:

	./chat <host> <port>
	./chat.lua <host> <port>
