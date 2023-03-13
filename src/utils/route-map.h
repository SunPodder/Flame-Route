#ifndef ROUTE_MAP_H
#define ROUTE_MAP_H

#include "http/route.h"

#pragma once
typedef struct route_entry_t {
    char *path;
    Route *route;
    struct route_entry_t *next;
} route_entry_t;

#pragma once
typedef struct {
    route_entry_t **entries;
} RouteMap;


RouteMap *route_map_create(unsigned int TABLE_SIZE);
Route *route_map_get(RouteMap* RouteMap, char* path);
void route_map_set(RouteMap* RouteMap, char* path, Route* route);
void route_map_dump(RouteMap* RouteMap);

#endif // ROUTE_MAP_H

