#ifndef METHOD_H
#define METHOD_H
#pragma once
typedef enum HTTPMethod {
	GET,
	POST,
	PUT,
	DELETE,
	HEAD,
	OPTIONS,
	PATCH,
	TRACE,
	CONNECT,
	UNKNOWN
} HTTPMethod;
#endif // METHOD_H
