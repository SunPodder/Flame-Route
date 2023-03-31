set(INJA_DOWNLOAD_URL "https://github.com/pantor/inja/releases/download/v3.4.0/inja.hpp")
set(INJA_DOWNLOAD_PATH "${CMAKE_CURRENT_SOURCE_DIR}/inja.hpp")

if(NOT EXISTS ${INJA_DOWNLOAD_PATH})
    message(STATUS "Downloading inja.hpp from ${INJA_DOWNLOAD_URL}")
    file(DOWNLOAD ${INJA_DOWNLOAD_URL} ${INJA_DOWNLOAD_PATH})
    message(STATUS "Patching inja.hpp")

    execute_process(
        COMMAND "patch" "--forward" "--silent" "-u" "inja.hpp" "-i" "inja.hpp.patch"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        RESULT_VARIABLE PATCH_RESULT
    )
    if(PATCH_RESULT)
        message(FATAL_ERROR "Failed to patch to ${CMAKE_CURRENT_SOURCE_DIR}/inja.hpp\nManually apply it with\n```\ncd <path to flame-route>\ncd src/templates\npatch -u inja.hpp -i inja.hpp.patch\n```")
    endif()
endif()

add_library(TEMPLATES STATIC jinja.cpp)

set(INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/include)

File(GLOB TEMPLATE_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
install(FILES ${TEMPLATE_HEADERS} DESTINATION ${INCLUDE_DIR}/templates)

