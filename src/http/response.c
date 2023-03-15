#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/sendfile.h>
#include <time.h>
#include <unistd.h>
#include <bits/seek_constants.h>
#include <string.h>
#include <http/response.h>


char *ServerName = "C HTTP Server/1.0";
char* HTTPStatusLine = "HTTP/1.1 %d %s\n\r";

/*
 * Internal helper function
 * @return: current time in string format
 */
char* getResponseTime(){
    time_t date;
    time(&date);
    return ctime(&date);
}

/*
 * Internal helper function
 * @param code: HTTP status code
 * @return: HTTP status string
 */
char* getHTTPStatusFromCode(int code){
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


/*
 * @param socket: socket to send the response to
 * @param contentSize: size of the content to be sent
 * @param mimeType: mime type of the content
 */
void HTTPSendHeaders(int socket, int contentSize, char* mimeType){
    char *date = getResponseTime();

    char* raw = "Server: %s\n\r"
        "Content-Length: %d\n\r"
        "Content-Type: %s\n\r"
        "Date: %s\n\r";

    char *response = (char*)malloc(strlen(raw)
            + strlen(ServerName)
            + strlen(date)
            + strlen(mimeType) + 100);

    sprintf(response, raw, ServerName, contentSize, mimeType, date);

    send(socket, response, strlen(response), 0);
    send(socket, "\n", 1, 0);
    free(response);
}


/*
 * @param socket: socket to send the response to
 * @param mimeType: mime type of the content
 * @param fd: file descriptor of the file to be sent
 */
void HTTPSendFile(int *socket, char *mimeType, int fd){
    int size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);

    HTTPSendHeaders(fd, size, mimeType);
    sendfile(*socket, fd, 0, size);
}

/*
 * @param socket: socket to send the response to
 * @param response: HTTPResponse object
 */
void HTTPSendResponse(int *socket, HTTPResponse *response){
    char* status = malloc(100);
    sprintf(status, HTTPStatusLine, response->status, getHTTPStatusFromCode(response->status));
    send(*socket, status, strlen(status), 0);
    free(status);

    HTTPSendHeaders(*socket, strlen(response->body), response->mimeType);
    send(*socket, "\n", 1, 0);
    send(*socket, response->body, strlen(response->body), 0);
    send(*socket, "\n\r\n\r", 4, 0);
}



void HTTP404(int *socket){
    HTTPResponse response;
    response.status = 404;
    response.body = "Not Found";
    response.mimeType = "text/plain";
    HTTPSendResponse(socket, &response);
}

void HTTP405(int *socket){
    HTTPResponse response;
    response.status = 405;
    response.body = "Method Not Allowed";
    response.mimeType = "text/plain";
    HTTPSendResponse(socket, &response);
}

void HTTP500(int *socket){
    HTTPResponse response;
    response.status = 500;
    response.body = "Internal Server Error";
    response.mimeType = "text/plain";
    HTTPSendResponse(socket, &response);
}
