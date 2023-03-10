cmake_policy(SET CMP0048 NEW)

Project(SimpleHTTPServer VERSION 1.0.3 LANGUAGES C)
cmake_minimum_required(VERSION 3.0)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_C_FLAGS "-Wall -Wextra")
set(CMAKE_C_FLAGS_DEBUG "-g")
set(CMAKE_C_FLAGS_RELEASE "-O3")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

add_subdirectory(src)


cmake_policy(SET CMP0079 NEW)
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)

install(TARGETS http-server DESTINATION bin)
