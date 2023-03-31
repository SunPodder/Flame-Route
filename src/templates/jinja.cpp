#include <string>
#include <templates/inja.hpp>
#include <templates/jinja.hpp>
#include <json/json.hpp>

/*
 * Constructor for Jinja
 * @param templateDir The directory to look for templates in
 */
Template::Jinja::Jinja(std::string templateDir) {
    if(templateDir[templateDir.length()-1] != '/')
        templateDir.append("/");
    this->templateDir = templateDir;
}

/*
 * Render a template with the given data
 * @param templateName The name of the template to render
 * @param data JSON Object to render the template with
 * @return The rendered template
 */
std::string Template::Jinja::render(std::string templateName, JSON data){
    std::string templatePath = this->templateDir + templateName;
    std::string rendered = this->env.render_file(templatePath, data);
    return rendered;
}
