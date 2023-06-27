set(JSON_LIB_URL
	"https://github.com/nlohmann/json/releases/download/v3.11.2/json.hpp")
set(JSON_DOWNLOAD_PATH "${CMAKE_CURRENT_SOURCE_DIR}/json_impl.hpp")

if(NOT EXISTS "${JSON_DOWNLOAD_PATH}")
	message("Downloading external JSON library from ${JSON_LIB_URL}")
	file(DOWNLOAD "${JSON_LIB_URL}" "${JSON_DOWNLOAD_PATH}")
endif()

file(GLOB JSON_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
install(FILES ${JSON_HEADERS} DESTINATION "${FLAME_INCLUDE_DIR}/json")
