add_executable(example
    main.c
)

target_link_libraries(example PUBLIC HTTP)
target_link_libraries(example PUBLIC UTILS)
target_link_libraries(example PUBLIC MIME)
