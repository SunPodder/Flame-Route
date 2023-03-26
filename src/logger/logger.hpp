#ifndef LOGGER_HPP
#define LOGGER_HPP
#include <ctime>
#include <string>
#include <iostream>
#include <type_traits>

namespace Logger {

    template<typename T>
    std::string __get_str(T t){
        std::string msg;
        if constexpr(std::is_class<T>::value){
            if constexpr(std::is_member_function_pointer
                    <decltype(&T::__str__)>::value){
                msg = t.__str__();        
            }else{
                msg = t;
            }
        }else{
            // check if const char
            if constexpr(std::is_same<T, const char*>::value){
                msg = t;
            }else{
                msg = std::to_string(t);
            }
        }
        return msg;
    }

    inline void __print(std::string msg, std::string color){
        std::time_t now = time(0);
        std::string dt = ctime(&now);
        dt.erase(dt.length() - 1);
        std::cout << color << "[" << dt << "] " << msg << "\033[0m" << std::endl;
    }
    
    template<typename T, typename... Args>
    void Log(T t, Args... args){
        std::string msg = "";
        msg += __get_str(t);
        (..., (msg += " " + __get_str(args)));

        __print(msg, "\033[0m");
    }

    template<typename T, typename... Args>
    void Error(T t, Args... args){
        std::time_t now = time(0);
        std::string dt = ctime(&now);
        dt.erase(dt.length() - 1);

        std::string msg = "";
        msg += __get_str(t);
        (..., (msg += " " + __get_str(args)));

        std::cerr << "\033[31m[" << dt << "] " << msg << "\033[0m" << std::endl;
    }

    template<typename T, typename... Args>
    void Debug(T t, Args... args){
        std::string msg = "";
        msg += __get_str(t);
        (..., (msg += " " + __get_str(args)));
        __print(msg, "\033[35m");
    }

    template<typename T, typename... Args>
    void Warn(T t, Args... args){
        std::string msg = "";
        msg += __get_str(t);
        (..., (msg += " " + __get_str(args)));
        __print(msg, "\033[33m");
    }

    template<typename T, typename... Args>
    void Info(T t, Args... args){
        std::string msg = "";
        msg += __get_str(t);
        (..., (msg += " " + __get_str(args)));
        __print(msg, "\033[36m");
    }

    template<typename T, typename... Args>
    void Success(T t, Args... args){
        std::string msg = "";
        msg += __get_str(t);
        (..., (msg += " " + __get_str(args)));
        __print(msg, "\033[32m");
    }
};
#endif

