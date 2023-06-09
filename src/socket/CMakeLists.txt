add_library(SOCKET STATIC socket.cpp)

install(FILES socket.hpp DESTINATION ${FLAME_INCLUDE_DIR}/socket)
