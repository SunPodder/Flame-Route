#ifndef LOGGER_HPP
#define LOGGER_HPP
#include "utils/utils.hpp"
#include <ctime>
#include <iostream>
#include <string>
#include <type_traits>
#include <utility>

class Logger {
  private:
	template <typename T, typename = void>
	struct has_str_impl : std::false_type {};

	template <typename T>
	struct has_str_impl<T, std::__void_t<decltype(&T::__str__)>>
		: std::true_type {};

	template <typename T> using has_str = has_str_impl<T>;
	template <typename T>
	inline constexpr static bool has_str_v = has_str<T>::value;

	/*
	 * check if the type has a __str__ method
	 * if it does, call it and return the result
	 * if it doesn't, check if it's a string or a char*
	 * if it is, return it
	 * if it isn't, convert it to a string and return it
	 */
	template <typename T> static std::string __get_str(T t) {
		std::string msg;

		if constexpr (has_str_v<T>) {
			msg = t.__str__();
		} else {
			if constexpr (std::is_same<T, const char*>::value ||
						  std::is_same<T, std::string>::value) {
				msg = t;
			} else {
				msg = std::to_string(t);
			}
		}

		return msg;
	}

	/*
	 * print the message with the given color
	 */
	static inline void __print(std::string msg, std::string color) {
		std::string dt = Flame::Utils::getCTime();
		dt.erase(dt.length() - 1);
		std::cout << "[" << dt << "] " << color << msg << "\033[0m"
				  << std::endl;
	}

  public:
	/*
	 * @param t: the first argument
	 * @param args: the rest of the arguments
	 * concatenate all the arguments into a string
	 * and print it
	 */
	template <typename T, typename... Args> static void Log(T t, Args... args) {
		std::string msg = "";
		msg += __get_str(t);
		(..., (msg += " " + __get_str(args)));

		__print(msg, "\033[0m");
	}

	template <typename T, typename... Args>
	static void Error(T t, Args... args) {
		std::string dt = Flame::Utils::getCTime();
		dt.erase(dt.length() - 1);

		std::string msg = "";
		msg += __get_str(t);
		(..., (msg += " " + __get_str(args)));

		std::cerr << "\033[31m[" << dt << "] " << msg << "\033[0m" << std::endl;
	}

	template <typename T, typename... Args>
	static void Debug(T t, Args... args) {
		std::string msg = "";
		msg += __get_str(t);
		(..., (msg += " " + __get_str(args)));
		__print(msg, "\033[35m");
	}

	template <typename T, typename... Args>
	static void Warn(T t, Args... args) {
		std::string msg = "";
		msg += __get_str(t);
		(..., (msg += " " + __get_str(args)));
		__print(msg, "\033[33m");
	}

	template <typename T, typename... Args>
	static void Info(T t, Args... args) {
		std::string msg = "";
		msg += __get_str(t);
		(..., (msg += " " + __get_str(args)));
		__print(msg, "\033[36m");
	}

	template <typename T, typename... Args>
	static void Success(T t, Args... args) {
		std::string msg = "";
		msg += __get_str(t);
		(..., (msg += " " + __get_str(args)));
		__print(msg, "\033[32m");
	}
};
#endif
