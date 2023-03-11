#include "method.h"
#include "response.h"
#include "request.h"

typedef struct {
    char* path;
    HTTPMethod methods[9];
    HTTPResponse (*callback)(HTTPRequest request, HTTPResponse response);
} Route;

