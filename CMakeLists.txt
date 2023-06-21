cmake_policy(SET CMP0048 NEW)
cmake_minimum_required(VERSION 3.0)

Project(FlameRoute VERSION 0.0.6 LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++17")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT DB_DRIVER)
	message(STATUS "DB_DRIVER not set, using sqlite3 as default")
	SET(DB_DRIVER "sqlite3")
else()
	message(STATUS "DB_DRIVER: ${DB_DRIVER}")
endif()

if(DB_DRIVER STREQUAL "sqlite3")
	add_compile_definitions(DB_SQLITE3)
elseif(DB_DRIVER STREQUAL "mysql")
	add_compile_definitions(DB_MYSQL)
elseif(DB_DRIVER STREQUAL "mongodb")
	add_compile_definitions(DB_MONGODB)
endif()


find_package(OpenSSL REQUIRED)
find_package(SQLite3 REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

set(FLAME_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/include/FlameRoute" CACHE INTERNAL "Include directory for FlameRoute")

add_subdirectory(src)
add_subdirectory(examples)

