CC=gcc
CFLAGS=-O2 -Wall -Wextra

bin/thue: src/thue.c
	mkdir -p bin
	$(CC) $(CFLAGS) -o bin/thue src/thue.c

install: bin/thue
	cp bin/thue /usr/local/bin

uninstall: /usr/local/bin/thue
	rm /usr/local/bin/thue

clean:
	rm -rf bin
