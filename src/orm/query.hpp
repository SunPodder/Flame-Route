#ifndef QUERY_HPP
#define QUERY_HPP

#include "orm/database.hpp"
#include <initializer_list>
#include <iostream>
#include <vector>

namespace Flame {
namespace ORM {
template <typename T> class QuerySet : public std::vector<T> {
  public:
	QuerySet(){};
	~QuerySet(){};
};

template <typename T> class Query {
  public:
	std::string query;
	Query(){};

	/*
	 * Filter query
	 * @param conditions: list of conditions to filter
	 * e.g. {"age >= 18", "age <= 40"}
	 * @return Query object
	 */
	Query<T>* filter(std::initializer_list<std::string> conditions) {
		std::string query = this->query + " WHERE ";
		for (auto condition : conditions) {
			query += condition + " AND ";
		}
		query.erase(query.end() - 4, query.end());
		this->query = query;
		return this;
	}

	/*
	 * Execute query
	 * @return QuerySet object
	 */
	QuerySet<T>* exec() {
		this->query += ";";

		Database* db = Database::getInstance();
		QuerySet<T>* qs = db->query<T>(this->query);

		// query no longer needed
		delete this;
		return qs;
	}
};
} // namespace ORM
} // namespace Flame
#endif
