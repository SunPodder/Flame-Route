#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <iostream>
#include <orm/database_exception.hpp>
#include <string>

#ifdef DB_SQLITE3
#include <orm/driver/sqlite3.hpp>
#include <sqlite3.h>
#endif

namespace Flame {
namespace ORM {
template <typename T> class QuerySet;

class Database {
  protected:
	Database(){};
	static Database* instance;

  public:
	~Database();
	// sqlite, mysql all will implement these same methods
	// except the query method
	static Database* getInstance();
	void exec(const std::string& sql);

	// query method is a Template
	// so extra work is needed here
	template <typename T> QuerySet<T>* query(const std::string& sql) {
#ifdef DB_SQLITE3
		return sqlite3_query<T>(sql, db);
#else
		return new QuerySet<T>();
#endif
	}

  private:
#ifdef DB_SQLITE3
	sqlite3* db;
#else
	sqlite3* db;
#endif
};

} // namespace ORM
} // namespace Flame

#endif // SQLITE3_HPP
