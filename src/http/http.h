#include <http/request.h>
#include <http/response.h>
#include <http/method.h>
#include <utils/route-map.h>


#pragma once
typedef struct {
    int port;
    char* address;
    RouteMap *routes;   // <char(path), Route>
    StringMap *static_routes;   // <char(path), char(file_path)>

    int _static_routes_count;
    int _routes_count;
} FlameServer;

FlameServer create_flame_server();
int ignite_server(FlameServer *server);

void register_route(
        FlameServer *server,
        char *path,
        HTTPMethod method[],
        HTTPResponse (*callback)(HTTPRequest *request, HTTPResponse *response));
void register_static_route(FlameServer *server, char *path, char *file_path);

