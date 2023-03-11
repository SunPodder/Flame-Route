#include "http/http.h"

HTTPResponse Home(HTTPRequest request, HTTPResponse response){
    response.body = "Index Page";
    return response;
}

HTTPResponse hello_world(HTTPRequest request, HTTPResponse response){
    response.body = "Hello World!";
    return response;
}

int main(){
    FlameServer server = create_flame_server();

    register_route(server, "/", (HTTPMethod[]){GET}, Home);
    register_route(server, "/hello", (HTTPMethod[]){GET, POST}, hello_world);

    ignite_server(server);
    return 0;
}
