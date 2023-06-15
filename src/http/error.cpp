#include <http/error.hpp>
#include <http/method.hpp>

const char* HTTPException::what() const throw() { return message.c_str(); }

const char* HTTPException::getURI() const throw() { return uri.c_str(); }

HTTPMethod HTTPException::getMethod() const throw() { return method; }

int HTTPException::getCode() const throw() { return code; }

const char* BadRequestException::getInfo() const throw() {
	return info.c_str();
}

const char* InternalServerErrorException::getInfo() const throw() {
	return info.c_str();
}

const char* NotImplementedException::getInfo() const throw() {
	return info.c_str();
}
