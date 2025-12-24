CC=gcc
CFLAGS=-O2 -Wall -Wextra -o

all: src/thue.c
	mkdir -p bin
	$(CC) $(CFLAGS) bin/thue src/thue.c

install: bin/thue
	cp bin/thue /usr/local/bin
