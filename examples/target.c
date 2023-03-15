#include "http/http.h"
#include "middlewares/csrf.h"
#include "user.h"


HTTPResponse Home(HTTPRequest request, HTTPResponse response){
    User user = get_user(request.cookie);
    response.body = render_page(.page="index", .user=user);
    return response;
}

HTTPResponse hello_world(HTTPRequest request, HTTPResponse response){
    response.body = "Hello World!";
    return response;
}

int main(){
    FlameServer server = create_flame_server();

    set_template_dir(server, "templates");
    set_http_middleware(server, csrf_middleware);

    register_route(server, "/", (HTTPMethod[]){GET}, Home);
    register_route(server, "/hello", (HTTPMethod[]){GET, POST}, hello_world);

    ignite_server(server);
    return 0;
}
