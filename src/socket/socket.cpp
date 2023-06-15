#include <arpa/inet.h>
#include <netinet/in.h>
#include <socket/socket.hpp>
#include <stdexcept>
#include <string>
#include <sys/sendfile.h>
#include <sys/socket.h>
#include <unistd.h>

Socket::Socket() { openned = false; }

Socket::Socket(int fd) {
	this->fd = fd;
	openned = true;
}

Socket::~Socket() {
	if (openned) {
		close();
	}
}

int Socket::open() {
	if (openned) {
		throw std::runtime_error("Socket is already openned");
	}
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == -1) {
		throw std::runtime_error("Error while opening socket");
	}
	openned = true;
	return 0;
}

int Socket::close() {
	if (!openned) {
		return 1;
	}
	if (::close(fd) == -1) {
		return -1;
	}
	openned = false;
	// if socket is closed, we don't need it anymore
	// but remeber to allocate it on heap
	delete this;
	return 0;
}

int Socket::bind(const std::string& host, int port) {
	if (!openned) {
		throw std::runtime_error("Socket is not openned");
	}
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(host.c_str());
	if (::bind(fd, (sockaddr*)&addr, sizeof(addr)) == -1) {
		throw std::runtime_error("Error while binding");
	}
	return 0;
}

int Socket::listen(int max_connections) {
	if (!openned) {
		throw std::runtime_error("Socket is not openned");
	}
	if (::listen(fd, max_connections) == -1) {
		throw std::runtime_error("Error while listening");
	}
	return 0;
}

Socket* Socket::accept() {
	if (!openned) {
		throw std::runtime_error("Socket is not openned");
	}
	sockaddr_in addr;
	socklen_t addr_len = sizeof(addr);
	int new_fd = ::accept(fd, (sockaddr*)&addr, &addr_len);
	if (new_fd == -1) {
		return new Socket(-1);
	}
	return new Socket(new_fd);
}

// connects to a server
// opposite to bind
int Socket::connect(const std::string& host, int port) {
	if (!openned) {
		throw std::runtime_error("Socket is not openned");
	}
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(host.c_str());
	if (::connect(fd, (sockaddr*)&addr, sizeof(addr)) == -1) {
		throw std::runtime_error("Error while connecting");
	}
	return 0;
}

int Socket::send(int to, const std::string& content, int flags) const {
	if (!openned) {
		return -1;
	}
	size_t sentBytes = 0;
	while (sentBytes < content.length()) {
		int result = ::send(to, content.c_str() + sentBytes,
							content.length() - sentBytes, flags);
		if (result == -1) {
			break;
		}
		sentBytes += result;
	}
	return sentBytes;
}

int Socket::sendfile(int to, int file) const {
	if (!openned) {
		return -1;
	}
	int size = lseek(file, 0, SEEK_END);
	(void)lseek(file, 0, SEEK_SET);
	off_t offset = 0;
	int remain = size;
	while (remain > 0) {
		int n = ::sendfile(to, file, &offset, remain);
		if (n == -1) {
			break;
		}
		remain -= n;
	}
	return size - remain;
}

std::string Socket::read(int from, std::string terminator, int flags) const {
	if (!openned) {
		throw std::runtime_error("Socket is not openned");
	}

	std::string buffer;
	int n;
	char buf[1024];

	// read in chunks of 1024 bytes
	// until terminator is found
	while ((n = ::recv(from, buf, 1024, flags)) > 0) {
		buffer.append(buf, n);
		if (buffer.find(terminator) != std::string::npos) {
			break;
		}
	}
	return buffer;
}

int Socket::getFd() const { return fd; }

bool Socket::isOpen() const { return openned; }

std::string Socket::getHost() const {
	sockaddr_in addr;
	socklen_t addr_len = sizeof(addr);
	if (getsockname(fd, (sockaddr*)&addr, &addr_len) == -1) {
		throw std::runtime_error("Error while getting host");
	}
	return inet_ntoa(addr.sin_addr);
}

std::string Socket::__str__() const {
	return "Socket(" + std::to_string(fd) + ")";
}
