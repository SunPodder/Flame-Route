#ifndef JINJA_HPP
#define JINJA_HPP
#include <iosfwd>
#include "../json/json.hpp"
#include "inja.hpp"

namespace Template {
class Jinja {
  public:
	/*
	 * Render a template with the given data
	 * @param templateName The name of the template to render
	 * @param data JSON Object to render the template with
	 * @return The rendered template
	 */
	std::string render(std::string templateName, JSON data);
	/*
	 * Constructor for Jinja
	 * @param templateDir The directory to look for templates in
	 */
	Jinja(std::string templateDir);

	/*
	 * The inja environment
	 * @see https://github.com/pantor/inja
	 */
	inja::Environment env;

  private:
	std::string templateDir;
};
} // namespace Template
#endif
