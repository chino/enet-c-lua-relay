SO=proxy.so
DIR=../../proxy
LIBS=-lenet
CFLAGS+=-I$(DIR)
SRC+=$(wildcard $(DIR)/*.c)
HEADERS+=$(wildcard $(DIR)/*.h)
