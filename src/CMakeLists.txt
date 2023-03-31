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

# Create final library to distribute
File(WRITE ${CMAKE_CURRENT_BINARY_DIR}/flame.cpp "")
add_library(FlameRoute STATIC flame.cpp)


target_link_libraries(FlameRoute HTTP MIME)



set(INCLUDE_DIR include/FlameRoute)

# Install headers
File(GLOB HTTP_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/http/*.hpp)
install(FILES ${HTTP_HEADERS} DESTINATION ${INCLUDE_DIR}/http)

# Install lib
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libFlameRoute.a DESTINATION lib)
