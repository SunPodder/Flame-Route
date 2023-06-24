#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>

namespace Flame {
namespace Utils {
/*
 * Split string
 * @param str
 * @param delimiter
 * @return vector<string>
 */
std::vector<std::string> split(const std::string& str, char delimiter);
/*
 * Trim string
 * @param str
 * @return string
 */
std::string trim(std::string str);
/*
 * Get current time
 * @return string
 */
std::string getCTime();
/*
 * Convert string to upper case
 * @param str
 * @return string
 */
std::string toUpper(std::string str);
} // namespace Utils
} // namespace Flame
#endif
