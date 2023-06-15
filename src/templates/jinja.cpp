#include <json/json.hpp>
#include <string>
#include <templates/inja.hpp>
#include <templates/jinja.hpp>

Template::Jinja::Jinja(std::string templateDir) {
	if (templateDir[templateDir.length() - 1] != '/')
		templateDir.append("/");
	this->templateDir = templateDir;
}

std::string Template::Jinja::render(std::string templateName, JSON data) {
	std::string templatePath = this->templateDir + templateName + ".html";
	std::string rendered = this->env.render_file(templatePath, data);
	return rendered;
}
