#ifndef DATABASE_EXCEPTION_HPP
#define DATABASE_EXCEPTION_HPP

#include <exception>
#include <string>

namespace Flame {
namespace ORM {
class DatabaseException : public std::exception {
  public:
	DatabaseException(const std::string& msg) : msg(msg) {}
	~DatabaseException() throw() {}

	const char* what() const throw() { return msg.c_str(); }

  private:
	std::string msg;
};
} // namespace ORM
} // namespace Flame
#endif // DATABASE_EXCEPTION_HPP
