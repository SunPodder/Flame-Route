#include <http/method.hpp>
#include <http/request.hpp>
#include <http/response.hpp>
#include <http/server.hpp>
#include <iosfwd>
#include <string.h>
#include <string>
#include <templates/jinja.hpp>

Template::Jinja jinja = Template::Jinja("./templates");

bool is_number(const std::string& s) {
	return (strspn(s.c_str(), "0123456789") == s.size());
}

void Home(HTTPRequest& request, HTTPResponse& response) {
	response.body = jinja.render("index", {{"name", "Flame"}});
}

void Login(HTTPRequest& request, HTTPResponse& response) {
	std::string username = std::string(request.body->map()["username"]);
	std::string password = std::string(request.body->map()["password"]);
	if (username == "admin" && password == "admin") {
		response.body = "Login Success";
	} else {
		response.body = "Login Failed";
	}
}

void User(HTTPRequest& request, HTTPResponse& response) {
	std::string user = request.params["user"];
	// check if user name int
	if (is_number(user)) {
		response.body = "ID: " + user;
		return;
	}
	response.body = "User: " + user;
}

int main() {
	FlameServer* server = new FlameServer();

	server->static_route("/static", "./");

	server->route("/", GET, Home);
	server->route("/login", POST, Login);
	server->route("/user/:user", GET, User);

	server->ignite("127.0.0.1", 8080);
	delete server;
	return 0;
}
