#ifndef INTERNAL_HPP
#define INTERNAL_HPP
#include <http/response.hpp>
#include <http/server.hpp>
#include <string>

struct thread_args {
    FlameServer& server;
    int socket;

    thread_args(FlameServer& server, int socket) : server(server), socket(socket) {}
};

std::string getResponseTime();
std::string getHTTPStatusFromCode(int code);

void sendResponse(const HTTPResponse& reponse);
void send404(HTTPResponse& response);
void send405(HTTPResponse& response);
void send500(HTTPResponse& response);
void sendFile(std::string path, HTTPResponse& response);

void* handle_connection(struct thread_args* args);
#endif
