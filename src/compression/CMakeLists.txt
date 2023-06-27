add_library(COMPRESSION OBJECT brotli.cpp)

file(GLOB HEADERS "*.hpp")
install(FILES ${HEADERS} DESTINATION ${FLAME_INCLUDE_DIR}/compression)
