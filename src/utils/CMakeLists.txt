add_library(UTILS OBJECT utils.cpp)

file(GLOB UTILS_HEADERS "*.hpp")
install(FILES ${UTILS_HEADERS} DESTINATION ${FLAME_INCLUDE_DIR}/utils)
