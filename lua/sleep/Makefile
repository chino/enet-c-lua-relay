include Makefile.inc

CC=gcc
SRC+=$(wildcard *.c)
OBJ+=$(patsubst %.c,%.o,$(SRC))
HEADERS+=$(wildcard *.h)

FLAGS+=-pipe -g -fstack-protector-all -shared -Wl,-soname,$(SO).1
CFLAGS+=-Wall -Wextra -D_FORTIFY_SOURCE=2 -I. -I/usr/include/lua

all: $(SO)

$(OBJ): $(HEADERS) $(SRC)
	$(CC) -c $(SRC) $(CFLAGS)

$(SO): $(OBJ)
	$(CC) $(FLAGS) -o $(SO) $(OBJ)

clean:
	$(RM) $(OBJ) $(SO) $(TAGS)

.PHONY: all clean
