#include <algorithm>
#include <ctype.h>
#include <http/error.hpp>
#include <http/request.hpp>
#include <http/utils.hpp>
#include <json/json.hpp>
#include <json/json_impl.hpp>
#include <sstream>
#include <stdio.h>
#include <string>
#include <unordered_map>
#include <utils/string-map.hpp>
#include <utils/utils.hpp>
#include <vector>

std::string http_unescape(std::string& str) {
	// TODO: implement
	return str;
}

template <typename T> void parse_urlencoded_data(T& map, std::string data) {
	std::istringstream data_stream(data);
	std::string data_part;
	while (std::getline(data_stream, data_part, '&')) {
		size_t delimiter_pos = data_part.find("=");
		if (delimiter_pos == std::string::npos) {
			continue;
		}
		std::string data_name =
			Flame::Utils::trim(data_part.substr(0, delimiter_pos));
		std::string data_value =
			Flame::Utils::trim(data_part.substr(delimiter_pos + 1));
		map[data_name] = data_value;
	}
}

RequestBody::RequestBody(std::string& body, HTTPRequest& request)
	: request(request), body(body) {
	this->_json = NULL;
	this->content_type = request.headers["Content-Type"];
	// transform content type to lowercase
	std::transform(this->content_type.begin(), this->content_type.end(),
				   this->content_type.begin(), ::tolower);
	this->string_map = StringMap();
}

JSON& RequestBody::json() {
	if (this->_json == NULL) {
		this->parse_json();
	}
	return this->_json;
}

StringMap& RequestBody::map() {
	if (this->string_map.empty()) {
		this->parse_string_map();
	}
	return this->string_map;
}

std::string& RequestBody::str() { return this->body; }

void RequestBody::parse_json() {
	this->_json = JSON();

	if (this->content_type == "application/json") {
		this->_json = JSON::parse(this->body);
	} else if (this->content_type == "application/x-www-form-urlencoded") {
		// example: name=John&age=23
		if (this->string_map.empty())
			parse_urlencoded_data(this->string_map, this->body);
		this->_json = JSON(this->string_map);
	} else if (this->content_type == "multipart/form-data") {
		// example: name=John; age=23
		// TODO: implement
		throw NotImplementedException(this->request.method, this->request.path,
									  "multipart/form-data not implemented");
	} else {
		throw BadRequestException(this->request.method, this->request.path,
								  "Content type not supported");
	}
}

void RequestBody::parse_string_map() {
	this->string_map = StringMap();
	if (this->body.empty())
		return;
	std::istringstream body_stream(this->body);

	if (this->content_type == "application/json") {
		std::string body_part;
		while (std::getline(body_stream, body_part, ',')) {
			size_t delimiter_pos = body_part.find(":");
			if (delimiter_pos == std::string::npos) {
				continue;
			}
			std::string body_name = body_part.substr(0, delimiter_pos);
			std::string body_value = body_part.substr(delimiter_pos + 1);
			this->string_map[body_name] = body_value;
		}

	} else if (this->content_type == "application/x-www-form-urlencoded") {
		// example: name=John&age=23
		parse_urlencoded_data(this->string_map, this->body);
	} else if (this->content_type == "multipart/form-data") {
		// example: name=John; age=23
		// TODO: implement
		throw NotImplementedException(this->request.method, this->request.path,
									  "multipart/form-data not implemented");
	} else {
		throw BadRequestException(this->request.method, this->request.path,
								  "Content type not supported");
	}
}

RequestBody::~RequestBody() {}

HTTPRequest::HTTPRequest(std::string request) { parse_request(request); }

// Parses an HTTP request from a string
void HTTPRequest::parse_request(const std::string& request_string) {
	// Split the request string into lines
	std::vector<std::string> lines;
	std::string line;
	std::istringstream request_stream(request_string);
	while (std::getline(request_stream, line)) {
		lines.push_back(line);
	}

	// Parse the request line
	std::vector<std::string> request_line_parts;
	std::istringstream request_line_stream(lines[0]);
	std::string request_part;
	while (request_line_stream >> request_part) {
		request_line_parts.push_back(request_part);
	}

	// Extract the request method and path
	this->method = getHTTPMethodFromStr(request_line_parts[0]);
	this->path = request_line_parts[1];

	// extract the query string if it exists
	size_t query_pos = this->path.find("?");
	std::string query_string;
	if (query_pos != std::string::npos) {
		query_string = this->path.substr(query_pos + 1);
		this->path = this->path.substr(0, query_pos);
	}

	// Parse the query string
	parse_urlencoded_data(this->query_params, query_string);
	// replace http escape characters
	for (auto& param : this->query_params) {
		param.second = http_unescape(param.second);
	}

	// Parse the headers
	for (size_t i = 1; i < lines.size(); i++) {
		const std::string& header_line = lines[i];
		size_t delimiter_pos = header_line.find(":");
		if (delimiter_pos == std::string::npos) {
			// Invalid header line
			continue;
		}
		std::string header_name =
			Flame::Utils::trim(header_line.substr(0, delimiter_pos));
		std::string header_value =
			Flame::Utils::trim(header_line.substr(delimiter_pos + 1));
		this->headers[header_name] = header_value;
	}

	// seperate cookies from headers
	if (this->headers.find("Cookie") != this->headers.end()) {
		std::istringstream cookie_stream(this->headers["Cookie"]);
		std::string cookie_part;
		while (std::getline(cookie_stream, cookie_part, ';')) {
			size_t delimiter_pos = cookie_part.find("=");
			if (delimiter_pos == std::string::npos) {
				continue;
			}
			std::string cookie_name =
				Flame::Utils::trim(cookie_part.substr(0, delimiter_pos));
			std::string cookie_value =
				Flame::Utils::trim(cookie_part.substr(delimiter_pos + 1));
			this->cookies[cookie_name] = cookie_value;
		}
	}

	// Parse the request body
	std::string body = "";
	if (headers.find("Content-Length") != headers.end()) {
		int content_length = std::stoi(headers["Content-Length"]);
		if (content_length > 0) {
			body = lines.back();
		}
	}

	this->body = new RequestBody(body, *this);
}

HTTPRequest::~HTTPRequest() { delete this->body; }
