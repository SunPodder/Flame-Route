#ifndef ROUTE_MAP_H
#define ROUTE_MAP_H
#include <iosfwd>
#include <unordered_map>
class Route;
typedef std::unordered_map<std::string, Route> RouteMap;
#endif
