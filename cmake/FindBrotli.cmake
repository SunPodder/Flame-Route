find_path(BROTLI_INCLUDES NAMES brotli/encode.h brotli/decode.h)

set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
find_library(BROTLI_ENC NAMES brotlienc brotlienc-static)
find_library(BROTLI_DEC NAMES brotlidec brotlidec-static)
find_library(BROTLI_COMMON NAMES brotlicommon brotlicommon-static)

if(BROTLI_INCLUDES AND BROTLI_ENC AND BROTLI_DEC AND BROTLI_COMMON)
	set(BROTLI_FOUND TRUE)
endif()

if(BROTLI_FOUND)
	if(NOT BROTLI_FIND_QUIETLY)
		message(STATUS "Found Brotli:\n- Includes: ${BROTLI_INCLUDES}\n- Libraries: ${BROTLI_ENC} ${BROTLI_DEC} ${BROTLI_COMMON}")
	endif()
else()
	if(BROTLI_FIND_REQUIRED)
		message(FATAL_ERROR "Could not find Brotli")
	endif()
endif()

add_library(Brotli::ENC STATIC IMPORTED GLOBAL)
set_target_properties(Brotli::ENC PROPERTIES
	IMPORTED_LOCATION "${BROTLI_ENC}"
)

add_library(Brotli::DEC STATIC IMPORTED GLOBAL)
set_target_properties(Brotli::DEC PROPERTIES
	IMPORTED_LOCATION "${BROTLI_DEC}"
)

add_library(Brotli::COMMON STATIC IMPORTED GLOBAL)
set_target_properties(Brotli::COMMON PROPERTIES
	IMPORTED_LOCATION "${BROTLI_COMMON}"
)

