#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <http/method.hpp>
#include <http/request.hpp>
#include <vector>
#include <sstream>

HTTPMethod get_method(const char *method) {
    if (strcmp(method, "GET") == 0) {
        return GET;
    } else if (strcmp(method, "POST") == 0) {
        return POST;
    } else if (strcmp(method, "PUT") == 0) {
        return PUT;
    } else if (strcmp(method, "DELETE") == 0) {
        return DELETE;
    } else if (strcmp(method, "HEAD") == 0) {
        return HEAD;
    } else if (strcmp(method, "OPTIONS") == 0) {
        return OPTIONS;
    } else if (strcmp(method, "CONNECT") == 0) {
        return CONNECT;
    } else if (strcmp(method, "TRACE") == 0) {
        return TRACE;
    } else if (strcmp(method, "PATCH") == 0) {
        return PATCH;
    } else {
        return GET;
    }
}

HTTPRequest::HTTPRequest(std::string request) {
    parse_request(request);
}


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
    this->method = ::get_method(request_line_parts[0].c_str());
    this->path = request_line_parts[1];

    // Parse the headers
    for (size_t i = 1; i < lines.size(); i++) {
        const std::string& header_line = lines[i];
        size_t delimiter_pos = header_line.find(":");
        if (delimiter_pos == std::string::npos) {
            // Invalid header line
            continue;
        }
        std::string header_name = header_line.substr(0, delimiter_pos);
        std::string header_value = header_line.substr(delimiter_pos + 1);
        this->headers[header_name] = header_value;
    }

    // Parse the request body
    std::string body = "";
    if (headers.find("Content-Length") != headers.end()) {
        int content_length = std::stoi(headers["Content-Length"]);
        if (content_length > 0) {
            body = lines.back();
        }
    }

}


