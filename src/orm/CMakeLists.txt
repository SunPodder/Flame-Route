SET(ORM_SRC
	orm.cpp
	driver/${DB_DRIVER}.cpp
)

add_library(ORM STATIC ${ORM_SRC})

install(FILES orm.hpp DESTINATION ${FLAME_INCLUDE_DIR}/orm)

