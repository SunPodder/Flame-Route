# Comnand line utility
add_executable(flame-route main.cpp)
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
add_subdirectory(compression)

# Create final library to distribute
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/flame.cpp "")
add_library(
	FlameRoute STATIC
	${CMAKE_CURRENT_BINARY_DIR}/flame.cpp
	$<TARGET_OBJECTS:HTTP>
	$<TARGET_OBJECTS:MIME>
	$<TARGET_OBJECTS:TEMPLATES>
	$<TARGET_OBJECTS:SOCKET>
	$<TARGET_OBJECTS:UTILS>
	$<TARGET_OBJECTS:COMPRESSION>)

find_package(Brotli REQUIRED)

set(MRI_FILE "${CMAKE_BINARY_DIR}/ar.mri")
# combine brotli and FlameRoute with ar
add_custom_command(
	TARGET FlameRoute
	POST_BUILD
	COMMAND
		${CMAKE_COMMAND} -E echo
		"create ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/libFlameRoute.a" > ${MRI_FILE}
	COMMAND ${CMAKE_COMMAND} -E echo "addlib $<TARGET_FILE:Brotli::ENC>" >>
			${MRI_FILE}
	COMMAND ${CMAKE_COMMAND} -E echo "addlib $<TARGET_FILE:Brotli::DEC>" >>
			${MRI_FILE}
	COMMAND ${CMAKE_COMMAND} -E echo "addlib $<TARGET_FILE:Brotli::COMMON>" >>
			${MRI_FILE}
	COMMAND ${CMAKE_COMMAND} -E echo "addlib $<TARGET_FILE:FlameRoute>" >>
			${MRI_FILE}
	COMMAND ${CMAKE_COMMAND} -E echo "save" >> ${MRI_FILE}
	COMMAND ${CMAKE_COMMAND} -E echo "end" >> ${MRI_FILE}
	COMMAND ${CMAKE_AR} -M < ${MRI_FILE}
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	COMMENT "Combining brotli and FlameRoute")

# Install lib
install(FILES ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/libFlameRoute.a DESTINATION lib)
