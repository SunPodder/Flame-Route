cmake_policy(SET CMP0048 NEW)
cmake_minimum_required(VERSION 3.0)

Project(FlameRoute VERSION 0.0.6 LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

add_subdirectory(src)
add_subdirectory(src/http)
add_subdirectory(src/mime)


File(WRITE ${CMAKE_CURRENT_BINARY_DIR}/flame.cpp "")
add_library(FlameRoute STATIC flame.cpp)
target_link_libraries(FlameRoute HTTP MIME)

add_subdirectory(examples)

find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)

install(TARGETS flame-route DESTINATION bin)

set(INCLUDE_DIR include/FlameRoute)

File(GLOB HTTP_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/http/*.hpp)
install(FILES ${HTTP_HEADERS} DESTINATION ${INCLUDE_DIR}/http)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libFlameRoute.a DESTINATION lib)
