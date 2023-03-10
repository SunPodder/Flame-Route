typedef struct {
    char* mimeType;
    char* body;
} HTTPResponse;

void HTTPSendFile(int socket, char* mimeType, int fd);
void HTTPRawResponse(int socket, char* mimeType, char* response);
