CC=gcc
CFLAGS=-Wall


build: tema3.c
	$(CC) $(CFLAGS) tema3.c -o tema3

run: build
	./tema3

clean:
	rm -f *.o exec