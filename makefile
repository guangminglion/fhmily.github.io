CC = cc -std=c99 -pedantic

OBJ = socket.o \
	  udc.o

BIN = socket \
	  udc

SUBMAKE = zlog

all: $(BIN)

$(BIN): %: %.o zlog
	$(CC) -L./zlog/lib -lzlog -o $@ $@.o

# compiles
%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

# deps output: cc -MM *.c
socket.o: socket.c zlog/include/zlog.h
udc.o: udc.c zlog/include/zlog.h

# zlog install path
ZLOG_PATH = $(shell pwd)/zlog

zlog:
	cd ./modules/zlog && $(MAKE) clean && $(MAKE) PREFIX=ZLOG_PATH && $(MAKE) PREFIX=ZLOG_PATH install

.PHONY: all zlog clean
clean:
	rm $(OBJ)
	rm $(BIN)
