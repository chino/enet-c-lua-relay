SO=proxy.so
DIR=../../proxy
CFLAGS+=-I$(DIR)
SRC+=$(wildcard $(DIR)/*.c)
HEADERS+=$(wildcard $(DIR)/*.h)
