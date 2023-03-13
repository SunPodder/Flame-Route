#include "http/http.h"
#include "http/method.h"
#include <http/route.h>

Route find_route(FlameServer server, char* path, HTTPMethod method){
    for(int i = 0; i < sizeof(server->routes)/sizeof(server->routes[0]); i++){
        if(strcmp(server.routes[i].path, path) == 0 && server.routes[i].method == method){
            return server->routes[i];
        }
    }
}
