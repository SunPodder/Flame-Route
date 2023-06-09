#include "http/error.hpp"
#include "http/route.hpp"
#include "socket/socket.hpp"
#include <cstddef>
#include <thread>
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
 * error handler methods should be called through the Server class
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
	socket = new Socket();
	(void)socket->open();
	(void)socket->bind(ip_addr, port);
	(void)socket->listen(10);

    if (callback != nullptr) {
        callback();
    }else{
        Logger::Success("Server started on " + ip_addr + ":" + std::to_string(port));
    }

    while (true) {
        Socket *client = socket->accept();
		
        if (client->getFd() < 0) {
			Logger::Info("Failed to accept client");
			delete client;
            continue;
        }
		
		struct thread_args *args = new struct thread_args(*this, client);	
		std::thread t(handle_connection, args);
		t.detach();
    }
}

Route::Route(){}

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

FlameServer::~FlameServer() {
	this->socket->close();
	for (auto const& route : this->routes) {
		delete &route.second;
	}
	this->routes.clear();
	this->static_routes.clear();
}

