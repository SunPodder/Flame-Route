#include <http/method.hpp>
#include <http/request.hpp>
#include <http/response.hpp>
#include <http/server.hpp>
#include <iosfwd>
#include <string>
#include <templates/jinja.hpp>

Template::Jinja jinja = Template::Jinja("./templates");

void Home(const HTTPRequest& request, HTTPResponse& response) {
	response.body = jinja.render("index", {{"name", "Flame"}});
}

void Login(const HTTPRequest& request, HTTPResponse& response) {
	std::string username =
		std::string(request.body->to_string_map()["username"]);
	std::string password =
		std::string(request.body->to_string_map()["password"]);
	if (username == "admin" && password == "admin") {
		response.body = "Login Success";
	} else {
		response.body = "Login Failed";
	}
}

int main() {
	FlameServer* server = new FlameServer();

	server->static_route("/static", "./");

	server->route("/", GET, Home);
	server->route("/login", POST, Login);

	server->ignite("127.0.0.1", 8080);
	delete server;
	return 0;
}
