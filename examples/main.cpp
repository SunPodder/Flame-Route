#include <http/method.hpp>
#include <http/request.hpp>
#include <http/response.hpp>
#include <http/server.hpp>
#include <templates/jinja.hpp>
#include <json/json.hpp>

Template::Jinja jinja = Template::Jinja("./templates");

void home(const HTTPRequest &request, HTTPResponse &response){
    response.body = jinja.render("index.html", {{"name", "Flame"}});
}

int main(){
    FlameServer *server = new FlameServer();

    server->static_route("/static", "./");

    server->route("/", {GET}, home);

    server->ignite("127.0.0.1", 8080);

    return 0;
}
