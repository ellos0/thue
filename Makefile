CC=gcc
CFLAGS=-O2 -Wall -Wextra -o

all: src/thue.c
		$(CC) $(CFLAGS) bin/thue src/thue.c 
