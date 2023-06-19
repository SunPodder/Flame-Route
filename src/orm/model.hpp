#ifndef MODEL_HPP
#define MODEL_HPP

#include <initializer_list>
#include <iostream>
#include <orm/query.hpp>
#include <string>

namespace Flame {
namespace ORM {

class BaseModel {
  public:
	BaseModel() {}
	virtual ~BaseModel() {}

	virtual void save() = 0;
	virtual void remove() = 0;
	virtual void update() = 0;
};

class Model : public BaseModel {
  protected:
	std::string table_name;

  public:
	Model();
	~Model();

	/*
	 * Select fields from table
	 * @param fields: list of fields to select
	 * @return Query object
	 */

	template <typename T>
	static Query<T>* select(std::string table_name,
							std::initializer_list<std::string> fields) {
		Query<T>* q = new Query<T>();
		std::string query = "SELECT ";
		for (auto field : fields) {
			query += field + ", ";
		}
		query.pop_back();
		query.pop_back();
		query += " FROM " + table_name;

		q->query = query;
		return q;
	}

	/*
	 * Select all fields from table
	 * @return Query object
	 */
	template <typename T> static Query<T>* all(std::string table_name) {
		Query<T>* q = new Query<T>();
		q->query = "SELECT * FROM " + table_name;
		return q;
	}

	void save(){};
	void remove(){};
	void update(){};
};

} // namespace ORM
} // namespace Flame

#endif // MODEL_HPP
