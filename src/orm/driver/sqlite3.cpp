#include "utils/string-map.hpp"
#include <orm/database.hpp>
#include <orm/query.hpp>
#include <sqlite3.h>

extern StringMap DB_CONFIG;

namespace Flame {
namespace ORM {

Database* Database::instance = NULL;

Database::~Database() { sqlite3_close(db); }

Database* Database::getInstance() {
	if (instance == NULL) {
		instance = new Database();
		int rc = sqlite3_open(DB_CONFIG["DB_PATH"].c_str(), &instance->db);
		if (rc) {
			throw DatabaseException(sqlite3_errmsg(instance->db));
		}
	}
	return instance;
}

void Database::exec(const std::string& sql) {
	char* errMsg;
	int rc = sqlite3_exec(db, sql.c_str(), NULL, NULL, &errMsg);
	if (rc != SQLITE_OK) {
		throw DatabaseException(errMsg);
	}
}

} // namespace ORM
} // namespace Flame
