#include <http/method.h>
#include <utils/string-map.h>

#pragma once
typedef struct {
    char *path;
    char *body;
    HTTPMethod method;
    StringMap *headers;
} HTTPRequest;

HTTPRequest *parse_request(char* request);
