CC = cc -std=c11 -pedantic

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



zlog:
	cd ./modules/zlog && $(MAKE) clean && $(MAKE) PREFIX=/Users/jack/Development/C/YID/zlog && $(MAKE) PREFIX=/Users/jack/Development/C/YID/zlog install

.PHONY: all zlog clean
clean:
	rm $(OBJ)
	rm $(BIN)