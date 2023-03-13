#pragma once
typedef struct {
    char* mimeType;
    char* body;
} HTTPResponse;


void HTTP200(int *socket);
void HTTP404(int *socket);
void HTTP500(int *socket);
void HTTP501(int *socket);

void HTTPSendFile(int socket, char* mimeType, int fd);
void HTTPSendResponse(int socket, char* mimeType, char* response);
