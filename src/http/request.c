#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "request.h"

HTTPRequest *parse_request(const char *request_line) {
    HTTPRequest *request = malloc(sizeof(HTTPRequest));
    request->method = malloc(sizeof(char) * 10);
    request->path = malloc(sizeof(char) * 100);
    sscanf(request_line, "%s %s %s", request->method, request->path);
    return request;
}
