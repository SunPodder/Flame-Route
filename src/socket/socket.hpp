#pragma once
#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

class Socket {
public:
	Socket();
	Socket(int fd);
	~Socket();
	int open();
	int close();
	int getFd() const;
	bool isOpen() const;
	
	int send(int to, const std::string &content, int flags = 0);
	int sendfile(int to, int file);
	std::string read(int from, std::string terminator = "\r\n\r\n", int flags = 0);
	std::string getHost();
	std::string __str__();

	// server side
	int bind(const std::string& address, int port);
	int listen(int max_connections);
	Socket *accept();

	// client side
	int connect(const std::string& host, int port);

private:
	int fd;
	bool openned;
};
#endif
