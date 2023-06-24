find_package(Brotli REQUIRED)

add_library(COMPRESSION STATIC brotli.cpp)
target_link_libraries(COMPRESSION Brotli::ENC Brotli::DEC)

file(GLOB HEADERS "*.hpp")
install(FILES ${HEADERS} DESTINATION ${FLAME_INCLUDE_DIR}/compression)

