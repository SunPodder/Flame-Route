#pragma once
#ifndef ERROR_HPP
#define ERROR_HPP
#include <exception>
#include <http/method.hpp>
#include <iosfwd>
#include <string>

/*
 * Base class for all HTTP exceptions
 * Users may extend this class to create their own exceptions
 * @param method: HTTP method used by the client
 * @param uri: URI requested by the client
 * @param code: HTTP status code to send to the client
 * @param message: Message to send to the client
 */
class HTTPException : public std::exception {
	HTTPMethod method;
	std::string uri;
	int code;
	std::string message;

  public:
	HTTPException(HTTPMethod method, std::string uri, int code,
				  std::string message)
		: method(method), uri(uri), code(code), message(message){};
	const char* what() const throw();
	const char* getURI() const throw();
	HTTPMethod getMethod() const throw();
	int getCode() const throw();
};

/*
 * HTTP 400 exception
 * Bad request
 * @param method: HTTP method used by the client
 * @param uri: URI requested by the client
 * @param @optional info: what went wrong
 */
class BadRequestException : public HTTPException {
	std::string info;

  public:
	BadRequestException(HTTPMethod method, std::string uri,
						std::string info = "")
		: HTTPException(method, uri, 400, "Bad request"), info(info){};
	const char* getInfo() const throw();
};

/*
 * HTTP 401 exception
 * Unauthorized
 * @param method: HTTP method used by the client
 * @param uri: URI requested by the client
 */
class UnauthorizedException : public HTTPException {
  public:
	UnauthorizedException(HTTPMethod method, std::string uri)
		: HTTPException(method, uri, 401, "Unauthorized"){};
};

/*
 * HTTP 403 exception
 * Forbidden
 * @param method: HTTP method used by the client
 * @param uri: URI requested by the client
 */
class ForbiddenException : public HTTPException {
  public:
	ForbiddenException(HTTPMethod method, std::string uri)
		: HTTPException(method, uri, 403, "Forbidden"){};
};

/*
 * HTTP 404 exception
 * Requested URI was not found
 * @param method: HTTP method used by the client
 * @param uri: URI requested by the client
 */
class URINotFoundException : public HTTPException {
  public:
	URINotFoundException(HTTPMethod method, std::string uri)
		: HTTPException(method, uri, 404, "URI not found"){};
};

/*
 * HTTP 405 exception
 * Requested method is not allowed on the requested URI
 * @param method: HTTP method used by the client
 * @param uri: URI requested by the client
 */
class MethodNotAllowedException : public HTTPException {
  public:
	MethodNotAllowedException(HTTPMethod method, std::string uri)
		: HTTPException(method, uri, 405, "Method not allowed"){};
};

/*
 * HTTP 500 exception
 * Internal server error
 * @param method: HTTP method used by the client
 * @param uri: URI requested by the client
 * @param @optional info: what went wrong
 */
class InternalServerErrorException : public HTTPException {
	std::string info;

  public:
	InternalServerErrorException(HTTPMethod method, std::string uri,
								 std::string info = "")
		: HTTPException(method, uri, 500, "Internal server error"),
		  info(info){};
	const char* getInfo() const throw();
};

/*
 * HTTP 501 exception
 * Not implemented
 * @param method: HTTP method used by the client
 * @param uri: URI requested by the client
 * @param @optional info: what is not implemented
 */
class NotImplementedException : public HTTPException {
	std::string info;

  public:
	NotImplementedException(HTTPMethod method, std::string uri,
							std::string info = "")
		: HTTPException(method, uri, 501, "Not implemented"), info(info){};
	const char* getInfo() const throw();
};

/*
 * HTTP 503 exception
 * Service unavailable
 * @param method: HTTP method used by the client
 * @param uri: URI requested by the client
 */
class ServiceUnavailableException : public HTTPException {
  public:
	ServiceUnavailableException(HTTPMethod method, std::string uri)
		: HTTPException(method, uri, 503, "Service unavailable"){};
};

#endif
