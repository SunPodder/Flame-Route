#ifndef INTERNAL_HPP
#define INTERNAL_HPP
#include <http/response.hpp>
#include <iosfwd>
#include <http/method.hpp>
#include <http/request.hpp>
class FlameServer;
class Socket;

struct thread_args {
    FlameServer &server;
    Socket *client;

    thread_args(FlameServer& server, Socket *client) : server(server), client(client) {}
};

std::string trim(std::string str);
std::string getResponseTime();
std::string getHTTPStatusFromCode(int code);
std::string getHTTPMethodFromEnum(HTTPMethod method);
HTTPMethod getHTTPMethodFromStr(std::string method_str);


void sendHeaders(Socket &server_fd, const HTTPResponse &response);
void sendResponse(Socket &server_fd, HTTPResponse &reponse);
void sendFile(Socket &server_fd, const std::string &path, const HTTPRequest &request, HTTPResponse &response);

void* handle_connection(struct thread_args* args);
#endif
