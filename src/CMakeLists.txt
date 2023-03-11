add_executable(flame-route
    main.c
)

target_link_libraries(flame-route PUBLIC HTTP)
target_link_libraries(flame-route PUBLIC UTILS)
target_link_libraries(flame-route PUBLIC MIME)
