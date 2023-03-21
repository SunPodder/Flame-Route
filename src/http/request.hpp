#ifndef REQUEST_H
#define REQUEST_H
#include <http/method.hpp>
#include <iostream>
#include <string>
#include <unordered_map>

class HTTPRequest{
    public:
        std::string path;
        std::string body;
        HTTPMethod method;
        std::unordered_map<std::string, std::string> query_params;
        std::unordered_map<std::string, std::string> cookies;
        std::unordered_map<std::string, std::string> headers;
        HTTPRequest(std::string request);

    private:
        void parse_request(const std::string& request);
};
#endif // REQUEST_H

