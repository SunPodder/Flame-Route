#ifndef REQUEST_H
#define REQUEST_H
#include "../json/json.hpp"
#include "../utils/string-map.hpp"
#include "method.hpp"
#include <iostream>

class HTTPRequest;

class RequestBody {
  private:
	JSON _json;
	StringMap string_map;
	HTTPRequest& request;
	std::string& body;
	std::string content_type;
	void parse_json();
	void parse_string_map();

  public:
	/*
	 * Request body as string
	 */
	std::string& str();
	/*
	 * Request body as JSON
	 */
	JSON& json();
	/*
	 * Request body as std::unordered_map<std::string, std::string>
	 */
	StringMap& map();
	RequestBody(std::string& body, HTTPRequest& request);
	~RequestBody();
};

class HTTPRequest {
  public:
	std::string path;
	HTTPMethod method;
	StringMap query_params;
	StringMap cookies;
	StringMap headers;
	StringMap params;
	RequestBody* body;
	HTTPRequest(std::string request);
	~HTTPRequest();

  private:
	void parse_request(const std::string& request);
};
#endif // REQUEST_H
