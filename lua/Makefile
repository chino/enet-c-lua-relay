CC=gcc

DIR=../proxy

FLAGS=-pipe -g -fstack-protector-all
CFLAGS+=-Wall -Wextra -D_FORTIFY_SOURCE=2 -I$(DIR) -I/usr/include/lua

LIBS=-lenet -llua 
INCLUDE=$(wildcard $(DIR)/*.h *.h)
SRC=$(wildcard $(DIR)/*.c *.c)
OBJS=$(patsubst %.c,%.o,$(SRC))

BIN=lua

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) -o $(BIN) $(OBJS) $(LDFLAGS) $(LIBS)

$(OBJS): $(INCLUDE) $(SRC)

clean:
	$(RM) $(OBJS) $(BIN) $(TAGS)

.PHONY: all clean
