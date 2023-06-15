#ifndef REQUEST_H
#define REQUEST_H
#include <http/method.hpp>
#include <iostream>
#include <json/json.hpp>
#include <utils/string-map.hpp>

class HTTPRequest;

class RequestBody {
  private:
	JSON json;
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
	JSON& to_json();
	/*
	 * Request body as std::unordered_map<std::string, std::string>
	 */
	StringMap& to_string_map();
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
	RequestBody* body;
	HTTPRequest(std::string request);
	~HTTPRequest();

  private:
	void parse_request(const std::string& request);
};
#endif // REQUEST_H
