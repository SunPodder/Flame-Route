#ifndef SERVER_H
#define SERVER_H
#include <string>
#include <utils/string-map.hpp>
#include <http/route.hpp>
#include <http/request.hpp>
#include <http/response.hpp>
#include <http/method.hpp>
#include <utils/route-map.hpp>
#include <iostream>

class FlameServer{
    public:
        RouteMap routes;
        StringMap static_routes;
        int ignite(std::string address, int port);
        void register_route(std::string path, const HTTPMethod (&methods)[9],
                void (*callback)(const HTTPRequest &request, HTTPResponse &response));
        void register_static_route(std::string path, std::string file_path);
        FlameServer();
};
#endif // FLAME_HTTP_H

