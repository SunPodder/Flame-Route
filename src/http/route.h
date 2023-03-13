#ifndef ROUTE_H
#define ROUTE_H

#include <http/method.h>
#include <http/response.h>
#include <http/request.h>

#pragma once
typedef struct {
    char* path;
    HTTPMethod methods[9];
    HTTPResponse (*callback)(HTTPRequest request, HTTPResponse response);
} Route;

#endif // ROUTE_H

