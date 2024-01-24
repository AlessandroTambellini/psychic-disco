CC=gcc
CFLAGS=-Wall
INSTALL_PATH=/usr/bin

all: pd-server client

pd-server: server.o program.o vm.o
	$(CC) $(CFLAGS) -o pd-server server.o program.o vm.o

client: client.o program.o
	$(CC) $(CFLAGS) -o client client.o program.o

clean:
	rm -f pd-server client *.o

install: pd-server
	echo Installing executable to ${INSTALL_PATH}
	mv pd-server ${INSTALL_PATH}/pd-server

uninstall:
	echo Removing executable from ${INSTALL_PATH}
	rm ${INSTALL_PATH}/pd-server
