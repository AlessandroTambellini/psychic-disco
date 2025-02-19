CC=clang
CFLAGS=-Wall

.INTERMEDIATE: tests.o ../program.o ../vm.o ../el.o

.PHONY: test

test: tests.o ../program.o ../vm.o ../el.o
	$(CC) $(CFLAGS) -o tests tests.o ../program.o ../vm.o ../el.o
	./tests

clean:
	rm -f tests *.o
