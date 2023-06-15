#include "user.hpp"
#include <http/method.hpp>
#include <http/request.hpp>
#include <http/response.hpp>
#include <http/server.hpp>
#include <middlewares/csrf.hpp>
#include <string>

void Home(const HTTPRequest& request, HTTPResponse& response) {
	User user = get_user(request.cookies);
	if (user)
		response.body = render_page("home", {.user = User});
	else
		response.redirect("/login");
}

void Login(const HTTPRequest& request, HTTPResponse& response) {
	std::string email = request.body["email"];
	std::string password = request.body["password"];
	if (request.method == POST) {
		if (check_credentials(email, password)) {
			response.cookie = generate_cookie(email);
			User user = get_user(request.cookies) response.body =
				render_page("home");
		} else {
			response.body = render_page("login", {.status = "Failed"});
		}
	} else {
		response.body = render_page("login")
	}
}

int main() {
	FlameServer* server = new FlameServer();

	server->set_template_dir("templates");
	server->set_http_middleware(csrf_middleware);

	server->register_route("/", {GET}, Home);
	server->register_route("/login", {GET, POST}, Login);

	server->ignite("127.0.0.1", 8080);
	return 0;
}
