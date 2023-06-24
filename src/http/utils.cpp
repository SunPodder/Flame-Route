#include <http/method.hpp>
#include <http/utils.hpp>
#include <string>
#include <utils/utils.hpp>

/*
 * Internal helper function
 * @param code: HTTP status code
 * @return: HTTP status string
 */
std::string getHTTPStatusFromCode(int code) {
	switch (code) {
	case 200:
		return "OK";
	case 404:
		return "Not Found";
	case 405:
		return "Method Not Allowed";
	case 500:
		return "Internal Server Error";
	default:
		return "Unknown";
	}
}

/*
 * Get HTTP Method string from HTTPMethod enum
 */
std::string getHTTPMethodFromEnum(HTTPMethod method) {
	switch (method) {
	case GET:
		return "GET";
	case POST:
		return "POST";
	case PUT:
		return "PUT";
	case DELETE:
		return "DELETE";
	case HEAD:
		return "HEAD";
	case OPTIONS:
		return "OPTIONS";
	case PATCH:
		return "PATCH";
	default:
		return "UNKNOWN";
	}
}

HTTPMethod getHTTPMethodFromStr(std::string method) {
	method = Flame::Utils::toUpper(method);

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
		return UNKNOWN;
	}
}
