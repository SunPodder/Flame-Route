#ifndef RESPONSE_H
#define RESPONSE_H
#include <iostream>
#include <utils/string-map.hpp>

class HTTPResponse {
  private:
	int _to;

  public:
	std::string mimeType;
	std::string body;
	StringMap cookie;
	StringMap headers;
	int status;
	int to() const;
	int file;
	HTTPResponse(int to);

	/*
	 * Redirect to another page
	 *
	 * @param location The location to redirect to
	 * @param status The status code to use
	 */
	void redirect(std::string location, int status = 302);
};
#endif // RESPONSE_H
