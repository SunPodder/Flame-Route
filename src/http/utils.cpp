#include <cctype>
#include <exception>
#include <fcntl.h>
#include <http/error.hpp>
#include <http/method.hpp>
#include <http/request.hpp>
#include <http/response.hpp>
#include <http/route.hpp>
#include <http/server.hpp>
#include <http/utils.hpp>
#include <logger/logger.hpp>
#include <mime/mime.hpp>
#include <regex>
#include <socket/socket.hpp>
#include <sstream>
#include <string>
#include <time.h>
#include <unistd.h>
#include <unordered_map>
#include <utility>
#include <utils/route-map.hpp>

std::string FLAME_VERSION = "0.0.5";

std::vector<std::string> split(const std::string& str, char delimiter) {
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream token_stream(str);
	while (std::getline(token_stream, token, delimiter)) {
		tokens.push_back(token);
	}
	return tokens;
}

/*
 * Check if a valid static route
 * if yes, send static file
 * else return 404
 * @param server: FlameServer instance
 * @param request: HTTPRequest instance
 * @param response: HTTPResponse instance
 */
void handle_static_or_404(FlameServer& server, const HTTPRequest& request,
						  HTTPResponse& response) {
	bool found = false;
	std::string path = request.path;
	// loop over all keys of static routes
	for (auto const& [k, v] : server.static_routes) {
		// if path starts with key, it is a static route
		if (path.find(k) == 0) {
			// remove leading slash
			path = path.substr(1);
			// replace url path with mapped file path
			std::string file_path = path.replace(0, k.length(), v);
			// send file
			found = true;
			sendFile(*server.socket, file_path, request, response);
			break;
		}
	}
	if (!found)
		throw URINotFoundException(request.method, request.path);
}

/*
 * Send HTTP headers
 * @param server: Server Socket instance
 * @param response: HTTPResponse instance
 */
void sendHeaders(Socket& server, const HTTPResponse& response) {
	std::string date = getResponseTime();

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

	std::string raw = "Server: FlameRoute/" + FLAME_VERSION + "\r\n" +
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

	server.send(response.to(), raw);
}

/*
 * Send a file
 */
void sendFile(Socket& server, const std::string& path,
			  const HTTPRequest& request, HTTPResponse& response) {
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

	/*
	 * TODO:
	 * Don't use hardcoded response code
	 */
	server.send(response.to(), "HTTP/1.1 200 OK\r\n");
	sendHeaders(server, response);
	server.send(response.to(), "\n");
	server.sendfile(response.to(), fd);
	server.send(response.to(), "\r\n\r\n");
	close(fd);
}

/*
 * Send response
 */
void sendResponse(Socket& server, HTTPResponse& response) {
	std::string status = "HTTP/1.1 " + std::to_string(response.status) + " " +
						 getHTTPStatusFromCode(response.status) + "\r\n";

	server.send(response.to(), status);
	sendHeaders(server, response);

	server.send(response.to(), "\n");
	server.send(response.to(), response.body);
	server.send(response.to(), "\r\n\r\n");
}

const Route* find_route(const FlameServer& server, HTTPRequest& request) {
	RouteMap::const_iterator it = server.routes.find(request.path);
	const Route* r;

	if (it != server.routes.end()) {
		// exact match found
		// check if method is allowed
		r = &it->second;
	} else {
		// check if path accepts a parameter
		// e.g. /user/:id
		// if yes, add it to request.params
		// and return the route
		for (auto const& [k, v] : server.routes) {
			if (k.find(":") != std::string::npos) {
				// url maybe like /user/:id
				// or /user/:id/more
				// or /user/:id/:name

				// split url and path by /
				// and check if they have same length
				// if yes, they are a match
				// and the parameter is the part of path

				std::vector<std::string> url_parts = split(k, '/');
				std::vector<std::string> path_parts = split(request.path, '/');
				if (url_parts.size() == path_parts.size()) {
					// check if they are a match
					bool match = true;
					for (unsigned long i = 0; i < url_parts.size(); i++) {
						if (url_parts[i] != path_parts[i]) {
							// if not, check if it is a parameter
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

/*
 * @param args: thread arguments
 * @args->server: FlameServer instance
 * @args->client: Client Socket instance
 */
void* handle_connection(struct thread_args* args) {
	HTTPRequest* request;
	HTTPResponse* response;
	const Route* route;

	try {
		std::string buffer = args->server.socket->read(args->client->getFd());
		request = new HTTPRequest(buffer);
		response = new HTTPResponse(args->client->getFd());

		route = find_route(args->server, *request);

		// if route is null
		// check if it is a static route
		// if not, return 404
		if (route == nullptr) {
			handle_static_or_404(args->server, *request, *response);
		} else {
			route->callback(*request, *response);
			sendResponse(*args->server.socket, *response);
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
	(void)args->client->close();
	delete args;
	return nullptr;
}

/*
 * Internal helper function
 * @return: current time in string format
 */
std::string getResponseTime() {
	time_t date;
	time(&date);
	return std::string(ctime(&date));
}

/*
 * Internal helper function
 * @param code: HTTP status code
 * @return: HTTP status string
 */
std::string getHTTPStatusFromCode(int code) {
	switch (code) {
	case 200:
		return "OK";
	case 404:
		return "Not Found";
	case 405:
		return "Method Not Allowed";
	case 500:
		return "Internal Server Error";
	default:
		return "Unknown";
	}
}

/*
 * Get HTTP Method string from HTTPMethod enum
 */
std::string getHTTPMethodFromEnum(HTTPMethod method) {
	switch (method) {
	case HTTPMethod::GET:
		return "GET";
	case HTTPMethod::POST:
		return "POST";
	case HTTPMethod::PUT:
		return "PUT";
	case HTTPMethod::DELETE:
		return "DELETE";
	case HTTPMethod::HEAD:
		return "HEAD";
	case HTTPMethod::OPTIONS:
		return "OPTIONS";
	case HTTPMethod::PATCH:
		return "PATCH";
	default:
		return "UNKNOWN";
	}
}

HTTPMethod getHTTPMethodFromStr(std::string method) {
	for (auto& c : method)
		c = toupper(c);

	if (method == "GET") {
		return GET;
	} else if (method == "POST") {
		return POST;
	} else if (method == "PUT") {
		return PUT;
	} else if (method == "DELETE") {
		return DELETE;
	} else if (method == "HEAD") {
		return HEAD;
	} else if (method == "OPTIONS") {
		return OPTIONS;
	} else if (method == "CONNECT") {
		return CONNECT;
	} else if (method == "TRACE") {
		return TRACE;
	} else if (method == "PATCH") {
		return PATCH;
	} else {
		return GET;
	}
}

std::string trim(std::string str) {
	str.erase(0, str.find_first_not_of(' ')); // prefixing spaces
	str.erase(str.find_last_not_of(' ') + 1); // surfixing spaces
	return str;
}
