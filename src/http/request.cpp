#include <ctype.h>
#include <sstream>
#include <stdio.h>
#include <vector>
#include <http/method.hpp>
#include <http/request.hpp>

HTTPMethod get_method(std::string method){
    for (auto & c: method) c = toupper(c);

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
    this->method = ::get_method(request_line_parts[0]);
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


