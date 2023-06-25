#ifndef SERVER_H
#define SERVER_H
#include "../utils/route-map.hpp"
#include "../utils/shared-queue.hpp"
#include "../utils/string-map.hpp"
#include "method.hpp"
#include <iostream>
class HTTPRequest;
class HTTPResponse;
class Socket;
class Route;

class FlameServer {
  public:
	Socket* socket;
	RouteMap routes;
	StringMap static_routes;

	/*
	 * Starts the server
	 *
	 * @param ip_addr: IP address to bind to
	 * @param port: Port to bind to
	 * @param max_threads: Maximum number of threads to spawn - default 10
	 * @optional param callback: Callback function to run om server ignition
	 */
	int ignite(std::string address, int port, int max_threads = 100,
			   void (*callback)() = nullptr);

	/*
	 * Define a HTTP Route
	 *
	 * @param path: Path of the route
	 * @param method: HTTP Method the route accepts
	 * @param callback: Callback function to run when the route is accessed
	 */
	void route(std::string path, const HTTPMethod& method,
			   void (*callback)(HTTPRequest& request, HTTPResponse& response));

	/*
	 * Define a HTTP Route
	 *
	 * @param path: Path of the route
	 * @param method: Array of HTTP methods the route accepts
	 * @param callback: Callback function to run when the route is accessed
	 */
	void route(std::string path, const HTTPMethod (&methods)[9],
			   void (*callback)(HTTPRequest& request, HTTPResponse& response));

	/*
	 * Define a static file route
	 *
	 * @param path: Path of the route
	 * @param file_path: Path of the files to serve
	 */
	void static_route(std::string path, std::string file_path);

	/*
	 * Define a static file route
	 * Serves files from the same directory as the specified route
	 * e.g. ./static/file will be accessible from /static/file
	 *
	 * @param path: Path of the route
	 */
	void static_route(std::string path);

	std::string version();

	std::string __str__();
	FlameServer();
	~FlameServer();

  private:
	int VERSION_MAJOR = 0;
	int VERSION_MINOR = 8;
	int VERSION_PATCH = 0;
	SharedQueue<Socket*> _connections;
	int _max_threads = 0;
	int _thread_count = 0;
	int _route_count = 0;
	int _static_route_count = 0;
	void* _handle_connection(Socket* client);
	const Route* _find_route(HTTPRequest& request);
	void _handle_static_or_404(const HTTPRequest& request,
							   HTTPResponse& response);
	void _send_response(HTTPRequest& request, HTTPResponse& response);
	void _send_headers(HTTPResponse& response);
	void _send_file(const std::string& path, const HTTPRequest& request,
					HTTPResponse& response);
};
#endif // FLAME_HTTP_H
