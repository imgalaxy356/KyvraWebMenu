CC = gcc
CFLAGS = -Wall -O2 -DUSE_OPENSSL -DOPENSSL_API_1_1
LDFLAGS = -lpthread -lssl -lcrypto -ldl

all:
	$(CC) main.c civetweb.c -o webmenu $(CFLAGS) $(LDFLAGS)

clean:
	rm -f webmenu
