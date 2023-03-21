#include <string>
#include <http/response.hpp>

/*
 * Constructor
 */
HTTPResponse::HTTPResponse(int to){
    this->mimeType = "text/html";
    this->body = "";
    this->cookie = "";
    this->status = 200;
    this->to = to;
    this->file = -99;
}

void HTTPResponse::redirect(std::string location){
    this->status = 302;
    this->body = "Found";
    this->mimeType = "text/html";
    this->headers["Location"] = location;
}

