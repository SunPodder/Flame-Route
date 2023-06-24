#include "utils.hpp"
#include <sstream>
#include <string>
#include <time.h>
#include <vector>

namespace Flame {
namespace Utils {

std::vector<std::string> split(const std::string& str, char delimiter) {
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream token_stream(str);
	while (std::getline(token_stream, token, delimiter)) {
		tokens.push_back(token);
	}
	return tokens;
}

std::string trim(std::string str) {
	str.erase(0, str.find_first_not_of(' ')); // prefixing spaces
	str.erase(str.find_last_not_of(' ') + 1); // surfixing spaces
	return str;
}

std::string getCTime() {
	time_t date;
	time(&date);
	char buffer[60];
	ctime_r(&date, buffer);
	return std::string(buffer);
}

std::string toUpper(std::string str) {
	for (auto& c : str) {
		c = toupper(c);
	}
	return str;
}
} // namespace Utils
} // namespace Flame
