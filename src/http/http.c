#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <bits/in_addr.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "http.h"


FlameServer create_flame_server(){
    FlameServer server;
    server.address = "127.0.0.1";
    server.port = 8080;
    return server;
}


static void *handle_connection(void* soc){
    // TODO:
    // 1. Read the request from the socket
    // 2. Parse the request
    // 3. Find the route
    // 4. check it the route http method matches the request method
    // 5. call the route callback
    // 6. send the response to the client
    // 8. if route is not found send 404
    // 9. if method is not allowed send 405
    // 10. if the route is static send the file
    // 11. close the socket

    int new_socket = *(int*)soc;
}

int ignite_server(FlameServer server){
    // TODO:
    // 1. Create a socket
    // 2. Bind the socket to the server address and port
    // 3. Listen for connections
    // 4. Accept connections
    // 5. transfer the connection to a new thread
    // 6. return 0 if the server is successfully ignited
    
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                &opt, sizeof(opt))){
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(server.address);
    address.sin_port = htons( server.port );

    // binding socket to the port
    if (bind(server_fd, (struct sockaddr *)&address,
                                 sizeof(address))<0){
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    // listen for connections
    if (listen(server_fd, 3) < 0){
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while(1){
        // accept connection
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                       (socklen_t*)&addrlen))<0){
            perror("accept");
            continue;
        }
        // create a new thread to handle the connection
        pthread_t thread_id;
        if( pthread_create( &thread_id, NULL,  handle_connection, (void*) &new_socket) < 0){
            perror("could not create thread");
            continue;
        }
    }

    return 0;
}

void register_route(FlameServer server, char *path,
        HTTPMethod method[], HTTPResponse (*callback)(
            HTTPRequest request, HTTPResponse response)){
    // TODO:
    // 1. Create a route struct
    // 2. Add the route to the server routes hashmap <path, route>
}

void register_static_route(FlameServer server, char *path, char *file_path){
    // TODO:
    // add the route to the server's static routes
}
