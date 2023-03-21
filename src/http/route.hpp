#ifndef ROUTE_H
#define ROUTE_H
#include <http/method.hpp>
#include <http/response.hpp>
#include <http/request.hpp>
#include <iostream>

class Route {
    public:
        std::string path;
        HTTPMethod methods[9];
        void (*callback)(const HTTPRequest &request, HTTPResponse &response);
        Route();
};

#endif // ROUTE_H

