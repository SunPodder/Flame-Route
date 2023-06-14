#include <http/server.hpp>
#include <http/utils.hpp>
#include <iostream>
#include <logger/logger.hpp>
#include <string>
#include <thread>
#include <unordered_map>
#include <http/method.hpp>
#include <http/route.hpp>
#include <socket/socket.hpp>
class HTTPRequest;
class HTTPResponse;


FlameServer::FlameServer() {
}

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

    // keep accepting connections
    // TODO: limit number of max connections
    // push connections to a queue if limit is reached
    // and process them later
    // might also accept connections in a seperate thread?
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


void FlameServer::static_route(std::string path){
    this->static_routes.insert({ path, path });
    this->_static_route_count++;
}

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

