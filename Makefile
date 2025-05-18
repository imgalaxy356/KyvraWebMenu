CC = gcc
CFLAGS = -Wall -O2
LDFLAGS = -lpthread

.PHONY: all clean

all:
	$(CC) $(CFLAGS) main.c civetweb.c -o webmenu $(LDFLAGS)

clean:
	rm -f webmenu
