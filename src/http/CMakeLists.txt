set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused -Werror")

add_library(HTTP STATIC server.cpp request.cpp response.cpp utils.cpp)

add_dependencies(HTTP MIME)

# Install headers
File(GLOB HTTP_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
list(REMOVE_ITEM HTTP_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/utils.hpp)

message("${FLAME_INCLUDE_DIR}")
install(FILES ${HTTP_HEADERS} DESTINATION "${FLAME_INCLUDE_DIR}/http")

