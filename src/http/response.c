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

char* getResponseTime(){
    time_t date;
    time(&date);
    return ctime(&date);
}

void HTTP200(int *socket){
    char *raw = "HTTP/1.1 200 OK\n\r";
    send(*socket, raw, strlen(raw), 0);
}

void HTTP404(int *socket){
    char *raw = "HTTP/1.1 404 Not Found\n\r";
    send(*socket, raw, strlen(raw), 0);
}

void HTTP405(int *socket){
    char *raw = "HTTP/1.1 405 Method Not Allowed\n\r";
    send(*socket, raw, strlen(raw), 0);
}

void HTTP500(int *socket){
    char *raw = "HTTP/1.1 500 Internal Server Error\n\r";
    send(*socket, raw, strlen(raw), 0);
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
    printf("Response Headers:\n%s", response);
    free(response);
}


/*
 * @param socket: socket to send the response to
 * @param mimeType: mime type of the content
 * @param fd: file descriptor of the file to be sent
 */
void HTTPSendFile(int socket, char *mimeType, int fd){
    int size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);

    HTTPSendHeaders(fd, size, mimeType);
    sendfile(socket, fd, 0, size);
}

/*
 * @param socket: socket to send the response to
 * @param mimeType: mime type of the content
 * @param response: content to be sent
 */
void HTTPSendResponse(int socket, char* mimeType,char* response){
    HTTPSendHeaders(socket, strlen(response), mimeType);
    send(socket, "\n", 1, 0);
    send(socket, response, strlen(response), 0);
}
