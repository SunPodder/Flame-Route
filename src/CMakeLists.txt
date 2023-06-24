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
add_subdirectory(socket)
add_subdirectory(logger)
add_subdirectory(json)
add_subdirectory(templates)
add_subdirectory(utils)


# Create final library to distribute
File(WRITE ${CMAKE_CURRENT_BINARY_DIR}/flame.cpp "")
add_library(FlameRoute STATIC flame.cpp)

target_link_libraries(FlameRoute HTTP MIME TEMPLATES SOCKET UTILS)

# Install lib
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libFlameRoute.a DESTINATION lib)

