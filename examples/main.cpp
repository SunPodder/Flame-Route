#include <http/method.hpp>
#include <http/request.hpp>
#include <http/response.hpp>
#include <http/server.hpp>

void home(const HTTPRequest &request, HTTPResponse &response){
    response.body = "<h1>Welcome to Home</h1>";
}

int main(){
    FlameServer *server = new FlameServer();

    server->static_route("/static", "./");

    server->route("/", {GET}, home);

    server->ignite("127.0.0.1", 8080);

    return 0;
}
