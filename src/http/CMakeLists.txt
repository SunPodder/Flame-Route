add_library(HTTP STATIC server.cpp request.cpp response.cpp utils.cpp)

add_dependencies(HTTP MIME)

set(INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/include)

# Install headers
File(GLOB HTTP_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
list(REMOVE_ITEM HTTP_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/utils.hpp)
install(FILES ${HTTP_HEADERS} DESTINATION ${INCLUDE_DIR}/http)

