#ifndef RESPONSE_H
#define RESPONSE_H
#include <iostream>
#include <utils/string-map.hpp>

class HTTPResponse{
    public:
        std::string mimeType;
        std::string body;
        std::string cookie;
        StringMap headers;
        int status;
        int to;
        int file;
        HTTPResponse(int to);
        void redirect(std::string location);
};
#endif // RESPONSE_H

