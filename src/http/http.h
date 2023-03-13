

#include <http/request.h>
#include <http/response.h>
#include <http/method.h>
#include <utils/route-map.h>

#pragma once
typedef struct {
    int port;
    char* address;

    // TODO:
    // Hashmap to store routes <path, Route>
    // Hashmap to store static routes <path, file_path>
    RouteMap *routes;
} FlameServer;

FlameServer create_flame_server();
int ignite_server(FlameServer *server);

void register_route(
        FlameServer *server,
        char *path,
        HTTPMethod method[],
        HTTPResponse (*callback)(HTTPRequest *request, HTTPResponse *response));
void register_static_route(FlameServer *server, char *path, char *file_path);

