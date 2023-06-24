#ifndef INTERNAL_HPP
#define INTERNAL_HPP
#include <http/method.hpp>
#include <iosfwd>

std::string getHTTPStatusFromCode(int code);
std::string getHTTPMethodFromEnum(HTTPMethod method);
HTTPMethod getHTTPMethodFromStr(std::string method_str);
#endif
