CC=gcc

DIR=../proxy

FLAGS=-pipe -g -fstack-protector-all
CFLAGS+=-Wall -Wextra -D_FORTIFY_SOURCE=2 -I$(DIR)

LIBS=-lenet
INCLUDE=$(wildcard $(DIR)/*.h)
SRC=$(wildcard $(DIR)/*.c *.c)
OBJS=$(patsubst %.c,%.o,$(SRC))

BIN=../chat

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) -o $(BIN) $(OBJS) $(LDFLAGS) $(LIBS)

$(OBJS): $(INCLUDE)

clean:
	$(RM) $(OBJS) $(BIN) $(TAGS)

.PHONY: all clean
