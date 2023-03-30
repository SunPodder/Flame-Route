#ifndef LOGGER_HPP
#define LOGGER_HPP
#include <ctime>
#include <string>
#include <iostream>
#include <type_traits>
#include <utility>

class Logger {
    private:
        template<typename T, typename = void>
        struct has_str_impl : std::false_type {};

        template <typename T>
        struct has_str_impl<T, std::__void_t<decltype(&T::__str__)> > : std::true_type {};
        
        template <typename T>
        using has_str = has_str_impl<T>;
        template <typename T>
        inline constexpr static bool has_str_v = has_str<T>::value;


        template<typename T>
        static std::string __get_str(T t){
            std::string msg;
            
            if constexpr(has_str_v<T>){
                msg = t.__str__();
            } else {
                if constexpr(std::is_same<T, const char*>::value || std::is_same<T, std::string>::value){
                    msg = t;
                } else {
                    msg = std::to_string(t);
                }
            }

            return msg;
        }


        static inline void __print(std::string msg, std::string color){
            std::time_t now = time(0);
            std::string dt = ctime(&now);
            dt.erase(dt.length() - 1);
            std::cout << "[" << dt << "] " << color << msg << "\033[0m" << std::endl;
        }
        

    public:
        template<typename T, typename... Args>
        static void Log(T t, Args... args){
            std::string msg = "";
            msg += __get_str(t);
            (..., (msg += " " + __get_str(args)));

            __print(msg, "\033[0m");
        }

        template<typename T, typename... Args>
        static void Error(T t, Args... args){
            std::time_t now = time(0);
            std::string dt = ctime(&now);
            dt.erase(dt.length() - 1);

            std::string msg = "";
            msg += __get_str(t);
            (..., (msg += " " + __get_str(args)));

            std::cerr << "\033[31m[" << dt << "] " << msg << "\033[0m" << std::endl;
        }

        template<typename T, typename... Args>
        static void Debug(T t, Args... args){
            std::string msg = "";
            msg += __get_str(t);
            (..., (msg += " " + __get_str(args)));
            __print(msg, "\033[35m");
        }

        template<typename T, typename... Args>
        static void Warn(T t, Args... args){
            std::string msg = "";
            msg += __get_str(t);
            (..., (msg += " " + __get_str(args)));
            __print(msg, "\033[33m");
        }

        template<typename T, typename... Args>
        static void Info(T t, Args... args){
            std::string msg = "";
            msg += __get_str(t);
            (..., (msg += " " + __get_str(args)));
            __print(msg, "\033[36m");
        }

        template<typename T, typename... Args>
        static void Success(T t, Args... args){
            std::string msg = "";
            msg += __get_str(t);
            (..., (msg += " " + __get_str(args)));
            __print(msg, "\033[32m");
        }
};
#endif
