find_path(BROTLI_INCLUDES NAMES brotli/encode.h brotli/decode.h)
find_library(BROTLI_ENC NAMES brotlienc)
find_library(BROTLI_DEC NAMES brotlidec)

if(BROTLI_INCLUDES AND BROTLI_ENC AND BROTLI_DEC)
	set(BROTLI_FOUND TRUE)
endif()

if(BROTLI_FOUND)
	if(NOT BROTLI_FIND_QUIETLY)
		message(STATUS "Found Brotli: ${BROTLI_LIBRARIES}")
	endif()
else()
	if(BROTLI_FIND_REQUIRED)
		message(FATAL_ERROR "Could not find Brotli")
	endif()
endif()

add_library(Brotli::ENC INTERFACE IMPORTED)
set_target_properties(Brotli::ENC PROPERTIES
	INTERFACE_LINK_LIBRARIES "${BROTLI_ENC}"
)

add_library(Brotli::DEC INTERFACE IMPORTED)
set_target_properties(Brotli::DEC PROPERTIES
	INTERFACE_LINK_LIBRARIES "${BROTLI_DEC}"
)

