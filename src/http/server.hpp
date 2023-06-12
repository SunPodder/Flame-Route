#ifndef SERVER_H
#define SERVER_H
#include <http/method.hpp>
#include <iostream>
#include <utils/route-map.hpp>
#include <utils/string-map.hpp> 
class HTTPRequest;
class HTTPResponse;
class Socket;

class FlameServer{
    public:
		Socket *socket;
        RouteMap routes;
        StringMap static_routes;
        int ignite(std::string address, int port, void (*callback)() = nullptr);
        void route(std::string path, const HTTPMethod (&methods)[9],
                void (*callback)(const HTTPRequest &request, HTTPResponse &response));
        void static_route(std::string path, std::string file_path);
        void static_route(std::string path);
        std::string __str__();
        FlameServer();
		~FlameServer();

    private:
        int _route_count = 0;
        int _static_route_count = 0;
};
#endif // FLAME_HTTP_H

