#include <cstddef>
#include <utils/route-map.hpp>
#include <utils/string-map.hpp>
#include <logger/logger.hpp>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <http/server.hpp>
#include <stdlib.h>
#include <sys/sendfile.h>
#include <unistd.h>
#include <http/response.hpp>
#include <mime/mime.hpp>
#include <http/request.hpp>
#include <pthread.h>
#include <http/utils.hpp>

/*
 * TODO:
 * send* methods should be called through the Server class
 * Builtin methods will be implemented in HTTPResponse.cpp file
 * Users will also be able to implement their own methods
 * So server should determine which method to call
 * e.g. A 404 response will call send404
 * but if there's a custom 404 method, it should call that
 */


FlameServer::FlameServer() {
}

/*
 * Run the server
 * @param ip_addr: IP address to bind to
 * @param port: Port to bind to
 * @param callback: Callback function to run om server ignition
 */
int FlameServer::ignite(std::string ip_addr, int port, void (*callback)()) {
    // create socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        std::cerr << "Failed to create socket" << std::endl;
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        std::cerr << "Failed to set socket options" << std::endl;
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(ip_addr.c_str());
    address.sin_port = htons(port);

    // bind socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "Failed to bind socket" << std::endl;
        exit(EXIT_FAILURE);
    }

    // start listening
    if (listen(server_fd, 3) < 0) {
        std::cerr << "Failed to listen on socket" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (callback != nullptr) {
        callback();
    }else{
        Logger::Success("Server started on " + ip_addr + ":" + std::to_string(port));
    }

    while (true) {
        // accept client
        int client_fd = accept(server_fd, NULL, NULL);
        if (client_fd < 0) {
            std::cerr << "Failed to accept client" << std::endl;
            continue;
        }

        // handle request in a new thread
        pthread_t thread;
        struct thread_args args(*this, client_fd);
        if (::pthread_create(&thread, NULL, (void *(*)(void*))handle_connection, &args) != 0) {
            std::cerr << "Failed to create thread" << std::endl;
            HTTPResponse *r = new HTTPResponse(client_fd);
            ::send500(*r);
            delete r;
            close(client_fd);
            continue;
        }
    }
}

Route::Route(){};

/*
 * Define a route
 * @param path: Path of the route
 * @param method: HTTP methods the route accepts
 * @param callback: Callback function to run when the route is accessed
 */
void FlameServer::route(std::string path, const HTTPMethod (&method)[9],
        void (*callback)(const HTTPRequest &request, HTTPResponse &response)) {
    Route *route = new Route();
    route->path = path;
    for (int i = 0; i < (int)(sizeof(method) / sizeof(HTTPMethod)); i++) {
        route->methods[i] = method[i];
    }
    route->callback = callback;
    this->routes.insert({ path, *route });
    this->_route_count++;
}

/*
 * Define a static route
 * @param path: Path of the route
 */
void FlameServer::static_route(std::string path){
    this->static_routes.insert({ path, path });
    this->_static_route_count++;
}

/*
 * Define a static route
 * @param path: Path of the route
 * @param file_path: Path of the files to serve
 */
void FlameServer::static_route(std::string path, std::string file_path) {
    this->static_routes.insert({ path, file_path });
    this->_static_route_count++;
}

std::string FlameServer::__str__() {
    std::string str = "<FlameServer - " + std::to_string(this->_route_count) + " routes, " + std::to_string(this->_static_route_count) + " static routes>";
   
    return str;
}
