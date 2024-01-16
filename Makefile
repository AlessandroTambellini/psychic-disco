CC=gcc
CFLAGS=-Wall

all: server client test

server: server.o program.o vm.o msg.o
	$(CC) $(CFLAGS) -o server server.o program.o vm.o msg.o

client: client.o program.o msg.o
	$(CC) $(CFLAGS) -o client client.o program.o msg.o

test: test.o program.o vm.o
	$(CC) $(CFLAGS) -o test test.o program.o vm.o

clean:
	rm -f server client test *.o
