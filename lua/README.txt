lua.c:

	example c program which embeds lua

	note that the stand alone lua interpreter is exactly the same thing as this

	it supports running a single lua file as the first argument

	thus you can also run the chat.lua program using it

lib/:

	contain modules which are compiled into shared objects (.so/.dll)

	they can be loaded into lua by using: require "name"

	they can also be directly compiled into the lua interpreter if desired.
