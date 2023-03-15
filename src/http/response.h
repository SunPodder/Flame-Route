#pragma once
typedef struct {
    char* mimeType;
    char* body;
    char* cookie;
    int status;
} HTTPResponse;


void HTTP404(int *socket);
void HTTP405(int *socket);
void HTTP500(int *socket);
void HTTPRedirect(int socket, char* mimeType, char* body, char* location);

void HTTPSendFile(int *socket, char* mimeType, int *fd);
void HTTPSendResponse(int *socket, HTTPResponse *response);
