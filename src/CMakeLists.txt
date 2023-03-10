add_executable(http-server
    main.c
)

add_subdirectory(http)
target_link_libraries(http-server PUBLIC HTTP)
add_subdirectory(utils)
target_link_libraries(http-server PUBLIC UTILS)
add_subdirectory(mime)
target_link_libraries(http-server PUBLIC MIME)
