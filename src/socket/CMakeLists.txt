add_library(SOCKET OBJECT socket.cpp)

install(FILES socket.hpp DESTINATION ${FLAME_INCLUDE_DIR}/socket)
