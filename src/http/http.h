#include "request.h"
#include "response.h"

typedef struct {
    int port;
    char* address;
} FlameServer;

typedef enum {
    GET,
    POST,
    PUT,
    DELETE,
    HEAD,
    OPTIONS,
    PATCH,
    TRACE,
    CONNECT
} HTTPMethod;

FlameServer create_flame_server();
int ignite_server(FlameServer server);

void register_route(
        FlameServer server,
        char *path,
        HTTPMethod method[],
        HTTPResponse (*callback)(HTTPRequest request, HTTPResponse response));
void register_static_route(FlameServer server, char *path, char *file_path);
