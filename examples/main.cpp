#include <http/method.hpp>
#include <http/request.hpp>
#include <http/response.hpp>
#include <http/server.hpp>
#include <iosfwd>
#include <orm/model.hpp>
#include <string>
#include <templates/jinja.hpp>

using namespace Flame;

Template::Jinja jinja = Template::Jinja("./templates");

class User : public ORM::Model {
  public:
	std::string username;
	std::string email;
	std::string password;

	static ORM::Query<User>* all() { return Model::all<User>("users"); }
};

class Post : public ORM::Model {
  public:
	std::string title;
	std::string content;

	static ORM::Query<Post>* all() { return Model::all<Post>("posts"); }
};

void Home(const HTTPRequest& request, HTTPResponse& response) {
	response.body = jinja.render("index", {{"name", "Flame"}});
}

void Login(const HTTPRequest& request, HTTPResponse& response) {
	std::string username =
		std::string(request.body->to_string_map()["username"]);
	std::string password =
		std::string(request.body->to_string_map()["password"]);

	// User user = User::all()->exec<User>();

	response.body = "Hello";
}

int main() {
	FlameServer* server = new FlameServer();

	server->static_route("/static", "./");

	server->route("/", {GET}, Home);
	server->route("/login", {POST}, Login);

	ORM::QuerySet<User>* users =
		User::all()->filter({"age >= 18", "age <= 40"})->exec();

	ORM::QuerySet<Post>* posts = Post::all()->exec();

	// server->ignite("127.0.0.1", 8080);
	delete server;
	return 0;
}
