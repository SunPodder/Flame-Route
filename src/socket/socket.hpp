#ifndef SOCKET_HPP
#define SOCKET_HPP
#include <iosfwd>
#include <string>

class Socket {
  public:
	/*
	 * Create socket
	 * **Allocate on heap**
	 */
	Socket();

	/*
	 * Create a socket object from
	 * an existing file descriptor
	 * **Allocate on heap**
	 * @param fd - file descriptor
	 */
	Socket(int fd);
	~Socket();

	/*
	 * Open socket
	 *
	 * @return 0 if success, -1 if error
	 * @throws runtime_error if socket is already openned
	 */
	int open();

	/*
	 * Close socket and delete the object
	 * @return 0 if success, -1 if error
	 * @throws runtime_error if socket is not openned
	 */
	int close();

	/*
	 * Get file descriptor of socket
	 * @return file descriptor
	 * @throws runtime_error if socket is not openned
	 */
	int getFd() const;

	/*
	 * Check if socket is openned
	 * @return true if socket is openned, false otherwise
	 */
	bool isOpen() const;

	/*
	 * Send content to socket
	 *
	 * @param to - socket to send to
	 * @param content - content to send
	 * @param flags - flags to pass to send (default 0)
	 * @return number of sent bytes
	 * usually returns content.length(), but may return less if error occured
	 */
	int send(int to, const std::string& content, int flags = 0) const;

	/*
	 * Send a file to socket using sendfile syscall
	 *
	 * @param to - socket to send to
	 * @param file - file descriptor to send
	 * @return number of sent bytes
	 * usually returns file size, but may return less if error occured
	 */
	int sendfile(int to, int file) const;

	/*
	 * Read from socket until terminator is found
	 *
	 * @param from - socket to read from
	 * @param terminator - string to stop reading at (default "\r\n\r\n")
	 * @param flags - flags to pass to recv (default 0)
	 * @return string read from socket
	 */
	std::string read(int from, std::string terminator = "\r\n\r\n",
					 int flags = 0) const;

	/*
	 * Get host name of socket
	 *
	 * @return host name
	 * @throws runtime_error if socket is not openned
	 */
	std::string getHost() const;
	std::string __str__() const;

	/*
	 * Server side method to bind socket to address and port
	 *
	 * @param host - host to connect to
	 * @param port - port to connect to
	 * @return 0 if success, -1 if error
	 * @throws runtime_error if socket is not openned
	 */
	int bind(const std::string& address, int port);

	/*
	 * Server side method to listen for connections
	 *
	 * @param max_connections - maximum number of connections to queue
	 * @return 0 if success, -1 if error
	 * @throws runtime_error if socket is not openned
	 */
	int listen(int max_connections);

	/*
	 * Accept connection
	 *
	 * @return new socket object for accepted connection
	 * @throws runtime_error if socket is not openned
	 */
	Socket* accept();

	/*
	 * Client side method to connect to server
	 *
	 * @param host - host to connect to
	 * @param port - port to connect to
	 * @return 0 if success, -1 if error
	 */
	int connect(const std::string& host, int port);

  private:
	int fd;
	bool openned;
};
#endif
