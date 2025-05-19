#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include "civetweb.h"  // adjust include path if needed
#include <unistd.h> // for usleep

#include <stdbool.h> // for bool

struct {
    bool aimbot;
} GameSetting = { false };
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool get_aimbot(void);
void set_aimbot(bool val);

#ifdef __cplusplus
}
#endif
// URL decode function (unchanged)
static void url_decode(char* dst, const char* src) {
    char a, b;
    while (*src) {
        if ((*src == '%') &&
            ((a = src[1]) && (b = src[2])) &&
            (isxdigit(a) && isxdigit(b))) {
            if (a >= 'a') a -= 'a' - 'A';
            if (a >= 'A') a -= ('A' - 10);
            else a -= '0';
            if (b >= 'a') b -= 'a' - 'A';
            if (b >= 'A') b -= ('A' - 10);
            else b -= '0';
            *dst++ = 16 * a + b;
            src += 3;
        }
        else if (*src == '+') {
            *dst++ = ' ';
            src++;
        }
        else {
            *dst++ = *src++;
        }
    }
    *dst = '\0';
}

// Extract param from POST body (unchanged)
static char* get_param_value(const char* data, const char* key) {
    static char decoded_value[128];
    const char* pos = strstr(data, key);
    if (!pos) return NULL;

    pos += strlen(key);
    if (*pos != '=') return NULL;
    pos++;

    const char* end = strchr(pos, '&');
    size_t len = end ? (size_t)(end - pos) : strlen(pos);

    if (len >= sizeof(decoded_value)) len = sizeof(decoded_value) - 1;

    char encoded[128];
    memcpy(encoded, pos, len);
    encoded[len] = '\0';

    url_decode(decoded_value, encoded);

    return decoded_value;
}

// POST handler for /toggle (unchanged)
int toggle_handler(struct mg_connection* conn, void* cbdata) {
    char post_data[1024];
    int post_data_len = mg_read(conn, post_data, sizeof(post_data) - 1);
    post_data[post_data_len] = '\0';

    char* val = get_param_value(post_data, "aimbot");
    if (val) {
        if (strcmp(val, "on") == 0) {
            set_aimbot(true);
            printf("Aimbot Enabled\n");
        }
        else if (strcmp(val, "off") == 0) {
            set_aimbot(false);
            printf("Aimbot Disabled\n");
        }
    }

    mg_printf(conn,
        "HTTP/1.1 303 See Other\r\n"
        "Location: /\r\n"
        "Content-Length: 0\r\n\r\n");
    return 1;
}

// New root handler to serve a simple status page with toggle form
int root_handler(struct mg_connection* conn, void* cbdata) {
    const char* page_template =
        "<html><head><title>Game Menu</title></head><body>"
        "<h1>Aimbot Status: %s</h1>"
        "<form method='POST' action='/toggle'>"
        "<button type='submit' name='aimbot' value='%s'>Turn %s</button>"
        "</form>"
        "</body></html>";

    const char* status = GameSetting.aimbot ? "Enabled" : "Disabled";
    const char* next_action_value = GameSetting.aimbot ? "off" : "on";
    const char* next_action_label = GameSetting.aimbot ? "Off" : "On";
const char* status = get_aimbot() ? "Enabled" : "Disabled";

    char page[512];
    snprintf(page, sizeof(page), page_template, status, next_action_value, next_action_label);

    mg_printf(conn,
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: %d\r\n"
        "\r\n"
        "%s",
        (int)strlen(page), page);

    return 1;
}

int main() {
    const char* port_env = getenv("PORT");
    char listen_port[64];
    if (!port_env) {
        snprintf(listen_port, sizeof(listen_port), "8080");
    }
    else {
        snprintf(listen_port, sizeof(listen_port), "%s", port_env);
    }

    const char* options[] = {
        "document_root", ".",
        "listening_ports", listen_port,
        "access_log_file", "-",
        NULL
    };

    struct mg_context* ctx = mg_start(NULL, 0, options);
    if (!ctx) {
        printf("Error starting Civetweb server\n");
        return 1;
    }

    // Use the root_handler for "/" instead of redirect_handler
    mg_set_request_handler(ctx, "/", root_handler, NULL);
    // You can remove or repurpose "/web" or others as needed
    // mg_set_request_handler(ctx, "/web", root_handler, NULL);
    mg_set_request_handler(ctx, "/toggle", toggle_handler, NULL);

    printf("Server started, listening on port %s\n", listen_port);
    printf("Serving / with game menu page\n");

    while (true) {
#ifdef _WIN32
        Sleep(1);
#else
        usleep(1000);
#endif
    }

    mg_stop(ctx);
    return 0;
}
