CC=gcc

FLAGS=-pipe -g -fstack-protector-all
CFLAGS+=-Wall -Wextra -D_FORTIFY_SOURCE=2 -I/usr/include/lua

LIBS=-llua 
INCLUDE=$(wildcard *.h)
SRC=$(wildcard *.c)
OBJS=$(patsubst %.c,%.o,$(SRC))

BIN=lua

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) -o $(BIN) $(OBJS) $(LDFLAGS) $(LIBS)

$(OBJS): $(INCLUDE) $(SRC)

clean:
	$(RM) $(OBJS) $(BIN) $(TAGS)

.PHONY: all clean
