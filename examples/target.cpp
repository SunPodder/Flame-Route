#include <http/server.hpp>
#include <http/method.hpp>
#include <middlewares/csrf.hpp>
#include <http/request.hpp>
#include <http/response.hpp>
#include <string>
#include "user.hpp"


void Home(const HTTPRequest &request, HTTPResponse &response){
    User user = get_user(request.cookie);
    response.body = render_page("home", {.user=User});
}

void Auth(const HTTPRequest &request, HTTPResponse &response){
    std::string email = request.body["email"];
    std::string password = request.body["password"];

    if(check_credentials(email, password)){
        response.cookie = generate_cookie(email);
        response.body = "Access granted!";
    }else{
        response.body = "Access denied!";
    }
}

int main(){
    FlameServer *server = new FlameServer();

    server->set_template_dir("templates");
    server->set_http_middleware(csrf_middleware);

    server->register_route("/", {GET}, Home);
    server->register_route("/auth", {GET, POST}, Auth);

    server->ignite_server("127.0.0.1", 8080);
    return 0;
}
