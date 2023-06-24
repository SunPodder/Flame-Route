add_library(UTILS utils.cpp)

file(GLOB UTILS_HEADERS "*.hpp")
install(FILES ${UTILS_HEADERS} DESTINATION ${FLAME_INCLUDE_DIR}/utils)

