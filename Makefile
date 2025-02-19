CC=clang
CFLAGS=-Wall
INSTALL_PATH=/usr/bin
SERVER_NAME=pd-server
TESTS_DIR=tests

.INTERMEDIATE: server.o program.o vm.o el.o client.o repl.o

.PHONY: test

all: server client repl

server: server.o program.o vm.o el.o
	$(CC) $(CFLAGS) -o server server.o program.o vm.o el.o

client: client.o program.o
	$(CC) $(CFLAGS) -o client client.o program.o

test:
	make -C $(TESTS_DIR) test

repl: repl.o program.o
	$(CC) $(CFLAGS) -o repl repl.o program.o

clean:
	rm -f server client repl *.o
	make -C $(TESTS_DIR) clean

install: server
	echo Installing executable to ${INSTALL_PATH}
	mv server ${INSTALL_PATH}/${PD_SERVER}

uninstall:
	echo Removing executable from ${INSTALL_PATH}
	rm ${INSTALL_PATH}/${PD_SERVER}
