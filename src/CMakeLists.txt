add_executable(flame-route
    main.c
)

add_subdirectory(http)
target_link_libraries(flame-route PUBLIC HTTP)
add_subdirectory(utils)
target_link_libraries(flame-route PUBLIC UTILS)
add_subdirectory(mime)
target_link_libraries(flame-route PUBLIC MIME)
