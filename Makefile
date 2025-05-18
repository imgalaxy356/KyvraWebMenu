CC = gcc
CFLAGS ?= -Wall -O2
LDFLAGS ?= -lpthread

all:
	$(CC) main.c civetweb.c -o webmenu $(CFLAGS) $(LDFLAGS)

clean:
	rm -f webmenu
