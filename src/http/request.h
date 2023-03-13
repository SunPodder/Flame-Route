#include <http/method.h>
#include <utils/string-map.h>

#pragma once
typedef struct {
    char *path;
    HTTPMethod method;
    char *body;
    StringMap *headers;
} HTTPRequest;

HTTPRequest* parse_request(char* request);
