include Makefile.inc

CC=gcc
SRC+= $(wildcard *.c)
OBJ+= $(patsubst %.c,%.o,$(SRC))
HEADERS+= $(wildcard *.h)

FLAGS+=-pipe -g -fstack-protector-all
CFLAGS+=-Wall -Wextra -D_FORTIFY_BINURCE=2 -I. -I/usr/include/lua

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(FLAGS) -o $(BIN) $(OBJ) $(LIBS)

$(OBJ): $(HEADERS) $(SRC)

clean:
	$(RM) $(OBJ) $(BIN) $(TAGS)

.PHONY: all clean
