#include <http/response.hpp>
#include <string>

/*
 * Constructor
 */
HTTPResponse::HTTPResponse(int to) {
	this->mimeType = "text/html";
	this->body = "";
	this->cookie = {};
	this->headers = {};
	this->status = 200;
	this->_to = to;
	this->file = -99;
}

int HTTPResponse::to() const { return this->_to; }

void HTTPResponse::redirect(std::string location, int status) {
	this->status = status;
	this->headers["Location"] = location;
}
