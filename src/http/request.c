#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <http/method.h>
#include <http/request.h>

HTTPMethod get_method(char *method) {
    if (strcmp(method, "GET") == 0) {
        return GET;
    } else if (strcmp(method, "POST") == 0) {
        return POST;
    } else if (strcmp(method, "PUT") == 0) {
        return PUT;
    } else if (strcmp(method, "DELETE") == 0) {
        return DELETE;
    } else if (strcmp(method, "HEAD") == 0) {
        return HEAD;
    } else if (strcmp(method, "OPTIONS") == 0) {
        return OPTIONS;
    } else if (strcmp(method, "CONNECT") == 0) {
        return CONNECT;
    } else if (strcmp(method, "TRACE") == 0) {
        return TRACE;
    } else if (strcmp(method, "PATCH") == 0) {
        return PATCH;
    } else {
        return 0;
    }
}

HTTPRequest *parse_request(char *request_string) {
    HTTPRequest *request = malloc(sizeof(HTTPRequest));
    request->path = malloc(sizeof(char) * 256);

    char *line = strtok(request_string, "\n\r");
    char *method = malloc(sizeof(char) * 10);
    sscanf(line, "%s %s", method, request->path);
    request->method = get_method(method);
    free(method);

    return request;
}
