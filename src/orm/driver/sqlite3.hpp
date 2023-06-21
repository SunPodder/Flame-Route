#ifndef SQLITE3_HPP
#define SQLITE3_HPP
#include <exception>
#include <iostream>
#include <orm/database_exception.hpp>
#include <sqlite3.h>
#include <string>

// forward declaration
namespace Flame {
namespace ORM {
template <typename T> class QuerySet;
} // namespace ORM
} // namespace Flame

template <typename T>
Flame::ORM::QuerySet<T>* sqlite3_query(const std::string& sql, sqlite3* db) {
	// read from database
	sqlite3_stmt* stmt;
	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
	if (rc != SQLITE_OK) {
		throw Flame::ORM::DatabaseException(sqlite3_errmsg(db));
	}

	int result = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
	if (result != SQLITE_OK) {
		throw Flame::ORM::DatabaseException(sqlite3_errmsg(db));
		return nullptr;
	}
	int cols = sqlite3_column_count(stmt);

	while (sqlite3_step(stmt) == SQLITE_ROW) {
		for (int col = 0; col < cols; col++) {
			std::cout << sqlite3_column_text(stmt, col) << " ";
		}
		std::cout << std::endl;
	}
	Flame::ORM::QuerySet<T>* qs = new Flame::ORM::QuerySet<T>();
	return qs;
}

#endif // SQLITE3_HPP
