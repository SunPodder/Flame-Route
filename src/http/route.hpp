#ifndef ROUTE_H
#define ROUTE_H
#include <http/method.hpp>
#include <iostream>
class HTTPRequest;
class HTTPResponse;

class Route {
  public:
	std::string path;
	HTTPMethod methods[9];
	void (*callback)(HTTPRequest& request, HTTPResponse& response);
	Route(){};
};

#endif // ROUTE_H
