#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/sendfile.h>
#include <fcntl.h>
#include <http/server.hpp>
#include <http/response.hpp>
#include <http/request.hpp>
#include <mime/mime.hpp>
#include <utils/utils.hpp>
#include <string>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>



std::string FLAME_VERSION = "0.0.5";

void handle_static_or_404(int socket, FlameServer& server, std::string path) {
    // loop over all keys of static routes
    for(auto const& [k, v] : server.static_routes) {
        // if path starts with key, it is a static route
        if (path.find(k) == 0) {
            // remove leading slash
            path = path.substr(1);
            // replace url path with mapped file path
            std::string file_path = path.replace(0, k.length(), v);
            // send file
            sendFile(file_path, *(new HTTPResponse(socket)));
            break;
        }
    }
}

/*
 * Send HTTP headers
 */
void sendHeaders(const HTTPResponse &response){
    std::string date = getResponseTime();

    int size;
    // If file is set, use file size
    if(response.file > 0){
        size = lseek(response.file, 0, SEEK_END);
        lseek(response.file, 0, SEEK_SET);
    }else{
        size = response.body.length();
    }

    // idk why but this is needed
    // maybe because of the 4 bytes of newlines
    // at the end ?!
    // though sendFile was woeking fine without this
    size += 4;

    std::string raw = "Server: FlameRoute/" + FLAME_VERSION + "\r\n"
        + "Content-Length: " + std::to_string(size) + "\r\n"
        + "Content-Type: " + response.mimeType + "\r\n"
        + "Date: " + date + "\r\n";

    int bytesSent = 0;
    while (bytesSent < raw.length()) {
        int result = send(response.to, raw.c_str() + bytesSent, raw.length() - bytesSent, 0);
        if (result == -1) {
            // Handle error
            break;
        }
        bytesSent += result;
    }   
}


/*
 * Send a file
 */
void sendFile(std::string path, HTTPResponse &response){
    if(access(path.c_str(), F_OK) == -1){
        send404(response);
        return;
    }
    int fd = open(path.c_str(), O_RDONLY);
    if(fd < 0){
        send500(response);
        return;
    }
    int size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    response.mimeType = getMimeType(path);
    response.file = fd;

    /*
     * TODO:
     * Don't use hardcoded response code
     */
    send(response.to, "HTTP/1.1 200 OK\r\n", 17, 0);
    sendHeaders(response);
    send(response.to, "\n", 1, 0);
    
    int remaining = size;
    while(remaining > 0){
        int sent = sendfile(response.to, fd, 0, remaining);
        if(sent < 0){
            std::cout << "Error sending file" << std::endl;
            send500(response);
            return;
        }
        remaining -= sent;
    }

    send(response.to, "\r\n\r\n", 4, 0);
    close(fd);
}

/*
 * Send response
 */
void sendResponse(const HTTPResponse &response){
    std::string status = "HTTP/1.1 " + std::to_string(response.status) + " " + getHTTPStatusFromCode(response.status) + "\r\n";
    send(response.to, status.c_str(), status.length(), 0);

    sendHeaders(response);
    send(response.to, "\n", 1, 0);
    
    int sentBytes = 0;
    while (sentBytes < response.body.length()) {
        int result = send(response.to, response.body.c_str() + sentBytes, response.body.length() - sentBytes, 0);
        if (result == -1) {
            // Handle error
            break;
        }
        sentBytes += result;
    }
    send(response.to, "\r\n\r\n", 4, 0);
}



void send404(HTTPResponse &response){
    response.status = 404;
    response.body = "Not Found";
    response.mimeType = "text/html";
    sendResponse(response);
}

void send405(HTTPResponse &response){
    response.status = 405;
    response.body = "Method Not Allowed";
    response.mimeType = "text/html";
    sendResponse(response);
}

void send500(HTTPResponse &response) {
    response.status = 500;
    response.body = "Internal Server Error";
    response.mimeType = "text/html";
    sendResponse(response);
}

std::string read_request(int socket){
    std::string buffer;
    int n;
    char buf[1024];
    while((n = recv(socket, buf, 1024, 0)) > 0){
        buffer.append(buf, n);
        if(buffer.find("\r\n\r\n") != std::string::npos){
            break;
        }
    }
    return buffer;
}

Route* find_route(FlameServer& server, HTTPRequest& request){
    RouteMap::iterator it = server.routes.find(request.path);
   if (it != server.routes.end()) {
       return &it->second;
    }
    return nullptr;
}

void* handle_connection(struct thread_args* args){
    std::string buffer = read_request(args->socket);
    HTTPRequest *request = new HTTPRequest(buffer);

    std::cout << "GET - " << request->path << std::endl;

    Route* route = find_route(args->server, *request);

    if (route == nullptr) {
        handle_static_or_404(args->socket, args->server, request->path);
        close(args->socket);
        return nullptr;
    }

    HTTPResponse *response = new HTTPResponse(args->socket);
    response->status = 200;
    response->mimeType = "text/html";

    route->callback(*request, *response);
    sendResponse(*response);

    close(args->socket);
    return nullptr;
}


/*
 * Internal helper function
 * @return: current time in string format
 */
std::string getResponseTime(){
    time_t date;
    time(&date);
    return std::string(ctime(&date));
}

/*
 * Internal helper function
 * @param code: HTTP status code
 * @return: HTTP status string
 */
std::string getHTTPStatusFromCode(int code){
    switch(code){
        case 200:
            return "OK";
        case 404:
            return "Not Found";
        case 405:
            return "Method Not Allowed";
        case 500:
            return "Internal Server Error";
        default:
            return "Unknown";
    }
}

