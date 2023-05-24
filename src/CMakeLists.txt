# Comnand line utility
add_executable(flame-route
    main.cpp
)
install(TARGETS flame-route DESTINATION bin)

#
# Framework
#
add_subdirectory(http)
add_subdirectory(mime)
add_subdirectory(json)
add_subdirectory(templates)


# Create final library to distribute
File(WRITE ${CMAKE_CURRENT_BINARY_DIR}/flame.cpp "")
add_library(FlameRoute STATIC flame.cpp)


target_link_libraries(FlameRoute HTTP MIME TEMPLATES)

set(INCLUDE_DIR include/FlameRoute)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/logger/logger.hpp DESTINATION ${INCLUDE_DIR}/logger)

# Install lib
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libFlameRoute.a DESTINATION lib)
