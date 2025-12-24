CC=gcc
CFLAGS=-O2 -Wall -Wextra -o

all: src/thue.c
	$(CC) $(CFLAGS) bin/thue src/thue.c

install: bin/thue
	cp bin/thue /usr/local/bin
