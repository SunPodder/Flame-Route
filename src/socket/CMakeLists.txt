add_library(SOCKET STATIC socket.cpp)

install(FILES socket.hpp DESTINATION ${INCLUDE_DIR}/socket)
