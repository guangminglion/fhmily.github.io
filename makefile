CC = cc -std=c11 -pedantic

OBJ = socket.o \
	  udc.o \
	  conf.o \
	  log.o

BIN = socket \
	  udc

SUBMAKE = zlog

all: $(BIN)

$(BIN): %: zlog conf.o log.o %.o
	$(CC) -o $@ $@.o conf.o log.o -L./zlog/lib -lzlog

# compiles
%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

# deps output: cc -MM *.c
log.o: log.c log.h zlog/include/zlog.h
socket.o: socket.c conf.h log.h zlog/include/zlog.h
udc.o: udc.c zlog/include/zlog.h

# zlog install path
ZLOG_PATH = $(shell pwd)/zlog

zlog:
	cd ./modules/zlog && $(MAKE) clean && $(MAKE) PREFIX=$(ZLOG_PATH) && $(MAKE) PREFIX=$(ZLOG_PATH) install

.PHONY: all zlog clean
clean:
	rm $(OBJ)
	rm $(BIN)
