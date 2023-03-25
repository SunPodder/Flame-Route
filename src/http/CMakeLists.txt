add_library(HTTP STATIC server.cpp request.cpp response.cpp ../utils/utils.cpp)

add_dependencies(HTTP MIME)

