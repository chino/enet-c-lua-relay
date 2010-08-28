BIN=net
DIR=../net
LIBS=-lenet
CFLAGS+=-I$(DIR)
SRC+=$(wildcard $(DIR)/*.c)
HEADERS+=$(wildcard $(DIR)/*.h)
