if(NOT DB_DRIVER)
	SET(DB_DRIVER "sqlite3")
endif()

if(DB_DRIVER STREQUAL "sqlite3")
	add_definitions(-DDB_SQLITE3)
elseif(DB_DRIVER STREQUAL "mysql")
	add_definitions(-DDB_MYSQL)
elseif(DB_DRIVER STREQUAL "mongodb")
	add_definitions(-DDB_MONGODB)
endif()

SET(ORM_SRC
	orm.cpp
	driver/${DB_DRIVER}.cpp
)


add_library(ORM STATIC ${ORM_SRC})

install(FILES orm.hpp DESTINATION ${FLAME_INCLUDE_DIR}/orm)

