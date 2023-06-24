#include "http/error.hpp"
#include "http/request.hpp"
#include "http/response.hpp"
#include "utils/route-map.hpp"
#include "utils/shared-queue.hpp"
#include <asm/fcntl.h>
#include <bits/seek_constants.h>
#include <exception>
#include <http/method.hpp>
#include <http/route.hpp>
#include <http/server.hpp>
#include <http/utils.hpp>
#include <iostream>
#include <logger/logger.hpp>
#include <mime/mime.hpp>
#include <socket/socket.hpp>
#include <stdio.h>
#include <string>
#include <thread>
#include <unistd.h>
#include <unordered_map>
#include <utility>
#include <utils/utils.hpp>
#include <vector>

FlameServer::FlameServer() { this->socket = new Socket(); }

std::string FlameServer::version() {
	using namespace std;
	return to_string(VERSION_MAJOR) + "." + to_string(VERSION_MINOR) + "." +
		   to_string(VERSION_PATCH);
}

int FlameServer::ignite(std::string ip_addr, int port, int max_threads,
						void (*callback)()) {
	this->_max_threads = max_threads;
	(void)socket->open();
	(void)socket->bind(ip_addr, port);
	(void)socket->listen(10);

	if (callback != nullptr) {
		callback();
	} else {
		Logger::Success("Server started on " + ip_addr + ":" +
						std::to_string(port));
	}

	while (true) {
		Socket* client = socket->accept();

		if (client->getFd() < 0) {
			Logger::Info("Failed to accept client");
			delete client;
			continue;
		}

		if (this->_thread_count >= this->_max_threads) {
			Logger::Info("Max threads reached, waiting for a thread to finish");
			this->_connections.push(client);
		} else {
			std::thread t([this, client] { _handle_connection(client); });
			t.detach();
			this->_thread_count++;
		}
	}
}

/*
 * @param args: thread arguments
 * @args->server: FlameServer instance
 * @args->client: Client Socket instance
 */
void* FlameServer::_handle_connection(Socket* client) {
	HTTPRequest* request;
	HTTPResponse* response;
	const Route* route;

	try {
		std::string buffer = this->socket->read(client->getFd());
		request = new HTTPRequest(buffer);
		response = new HTTPResponse(client->getFd());

		route = this->_find_route(*request);

		// if route is null
		// check if it is a static route
		// if not, return 404
		if (route == nullptr) {
			this->_handle_static_or_404(*request, *response);
		} else {
			route->callback(*request, *response);
			this->_send_response(*response);
		}

		std::string method = getHTTPMethodFromEnum(request->method);
		Logger::Log(method + " - " + request->path + " " +
					std::to_string(response->status) + " " +
					getHTTPStatusFromCode(response->status));
	} catch (const URINotFoundException& e) {
		Logger::Info(getHTTPMethodFromEnum(e.getMethod()), e.getURI());
	} catch (const MethodNotAllowedException& e) {
		Logger::Error(e.getMethod(), e.getURI(), e.getCode());
	} catch (const InternalServerErrorException& e) {
		Logger::Error(e.what());
	} catch (const std::exception& e) {
		// TODO: call error handler
		Logger::Error(e.what());
	}

	delete request;
	delete response;
	(void)client->close();
	this->_thread_count--;
	// handle pending connections
	if (this->_connections.size() > 0) {
		Socket* client = this->_connections.pop();
		std::thread t([this, client] { _handle_connection(client); });
		t.detach();
		this->_thread_count++;
	}
	return nullptr;
}

void FlameServer::_send_response(HTTPResponse& response) {
	std::string status = "HTTP/1.1 " + std::to_string(response.status) + " " +
						 getHTTPStatusFromCode(response.status) + "\r\n";
	Socket& server = *this->socket;

	server.send(response.to(), status);
	this->_send_headers(response);

	server.send(response.to(), "\n");
	server.send(response.to(), response.body);
	server.send(response.to(), "\r\n\r\n");
}

/*
 * Send HTTP headers
 * @param response: HTTPResponse instance
 */
void FlameServer::_send_headers(const HTTPResponse& response) {
	std::string date = Flame::Utils::getCTime();

	int size;
	// If file is set, use file size
	if (response.file > 0) {
		size = lseek(response.file, 0, SEEK_END);
		(void)lseek(response.file, 0, SEEK_SET);
	} else {
		size = response.body.length();
	}

	// idk why but this is needed
	// maybe because of the 4 bytes of newlines
	// at the end ??
	// though sendFile was woeking fine without this
	size += 4;

	std::string raw = "Server: FlameRoute/" + this->version() + "\r\n" +
					  "Content-Length: " + std::to_string(size) + "\r\n" +
					  "Content-Type: " + response.mimeType + "\r\n" +
					  "Date: " + date + "\r\n";

	// add user defined headers
	for (auto const& [k, v] : response.headers) {
		raw += k + ": " + v + "\r\n";
	}

	// add cookies
	for (auto const& [k, v] : response.cookie) {
		raw += "Set-Cookie: " + k + "=" + v + "\r\n";
	}

	this->socket->send(response.to(), raw);
}

/*
 * Send a file
 */
void FlameServer::_send_file(const std::string& path,
							 const HTTPRequest& request,
							 HTTPResponse& response) {
	// NOLINT
	if (access(path.c_str(), F_OK) == -1) {
		// file doesn't exist
		throw URINotFoundException(request.method, request.path);
		return;
	}
	int fd = open(path.c_str(), O_RDONLY);
	if (fd < 0) {
		throw InternalServerErrorException(request.method, request.path,
										   "Could not open file");
		return;
	}
	response.mimeType = getMimeType(path);
	response.file = fd;

	Socket& server = *this->socket;

	/*
	 * TODO:
	 * Don't use hardcoded response code
	 */
	server.send(response.to(), "HTTP/1.1 200 OK\r\n");
	this->_send_headers(response);
	server.send(response.to(), "\n");
	server.sendfile(response.to(), fd);
	server.send(response.to(), "\r\n\r\n");
	::close(fd);
}

void FlameServer::_handle_static_or_404(const HTTPRequest& request,
										HTTPResponse& response) {
	bool found = false;
	std::string path = request.path;
	// loop over all keys of static routes
	for (auto const& [k, v] : this->static_routes) {
		// if path starts with key, it is a static route
		if (path.find(k) == 0) {
			// remove leading slash
			path = path.substr(1);
			// replace url path with mapped file path
			std::string file_path = path.replace(0, k.length(), v);
			// send file
			found = true;
			this->_send_file(file_path, request, response);
			break;
		}
	}
	if (!found)
		throw URINotFoundException(request.method, request.path);
}

const Route* FlameServer::_find_route(HTTPRequest& request) {
	RouteMap::const_iterator it = this->routes.find(request.path);
	const Route* r;

	if (it != this->routes.end()) {
		// exact match found
		r = &it->second;
	} else {
		// check if path accepts a parameter
		// url maybe like /user/:id
		// or /user/:id/more
		// or /user/:id/:name
		// if yes, add it to request.params
		for (auto const& [k, v] : this->routes) {
			if (k.find(":") != std::string::npos) {
				// split url and path by /
				// if length is same, they are a match
				// and the parameter is the part of path

				std::vector<std::string> url_parts =
					Flame::Utils::split(k, '/');
				std::vector<std::string> path_parts =
					Flame::Utils::split(request.path, '/');
				if (url_parts.size() == path_parts.size()) {
					bool match = true;
					for (unsigned long i = 0; i < url_parts.size(); i++) {
						if (url_parts[i] != path_parts[i]) {
							// check if it is a parameter
							if (url_parts[i].find(":") != std::string::npos) {
								// it is a parameter
								// add it to request.params
								// and continue
								std::string key = url_parts[i].substr(1);
								request.params[key] = path_parts[i];
								continue;
							}
							// if not, it is not a match
							match = false;
							break;
						}
					}
					if (match) {
						r = &v;
						break;
					}
				}
			}
		}
	}

	if (r != nullptr) {
		// check if method is allowed
		for (auto const& m : r->methods) {
			if (m == request.method) {
				return r;
			}
		}
		throw MethodNotAllowedException(request.method, request.path);
	}
	return nullptr;
}

void FlameServer::route(std::string path, const HTTPMethod& method,
						void (*callback)(HTTPRequest& request,
										 HTTPResponse& response)) {
	HTTPMethod methods[9] = {method};
	this->route(path, methods, callback);
}

void FlameServer::route(std::string path, const HTTPMethod (&method)[9],
						void (*callback)(HTTPRequest& request,
										 HTTPResponse& response)) {
	Route* route = new Route();
	route->path = path;
	for (int i = 0; i < (int)(sizeof(method) / sizeof(HTTPMethod)); i++) {
		route->methods[i] = method[i];
	}
	route->callback = callback;
	this->routes.insert({path, *route});
	this->_route_count++;
}

void FlameServer::static_route(std::string path) {
	this->static_routes.insert({path, path});
	this->_static_route_count++;
}

void FlameServer::static_route(std::string path, std::string file_path) {
	this->static_routes.insert({path, file_path});
	this->_static_route_count++;
}

std::string FlameServer::__str__() {
	std::string str = "<FlameServer - " + std::to_string(this->_route_count) +
					  " routes, " + std::to_string(this->_static_route_count) +
					  " static routes>";

	return str;
}

FlameServer::~FlameServer() {
	if (this->socket != nullptr)
		this->socket->close();
	for (auto const& route : this->routes) {
		delete &route.second;
	}
	this->routes.clear();
	this->static_routes.clear();
}
