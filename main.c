#include "civetweb.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // for sleep()

static int menu_handler(struct mg_connection *conn, void *ignored) {
    mg_printf(conn,
        "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
        "<!DOCTYPE html><html><head><title>DMA Menu</title></head><body>"
        "<h1>DMA Cheat Menu</h1>"
        "<form>"
        "Aimbot: <input type='checkbox' name='aimbot'><br><br>"
        "Smoothing: <input type='range' min='1' max='100' value='25'><br><br>"
        "ESP: <input type='checkbox' name='esp'><br><br>"
        "<input type='submit' value='Apply'>"
        "</form>"
        "</body></html>");
    return 1;  // Return 1 to indicate handled request
}

int main(void) {
    const char *options[] = {
        "listening_ports", "8080",
        "num_threads", "4",
        // "document_root", ".",  // Uncomment if you want static files served
        0
    };

    struct mg_callbacks callbacks;
    memset(&callbacks, 0, sizeof(callbacks));

    struct mg_context *ctx = mg_start(&callbacks, NULL, options);
    if (!ctx) {
        fprintf(stderr, "Failed to start CivetWeb server.\n");
        return 1;
    }

    mg_set_request_handler(ctx, "/", menu_handler, NULL);

    printf("Server started on port 8080.\n");
    while (1) {
        sleep(1);
    }

    mg_stop(ctx);
    return 0;
}
