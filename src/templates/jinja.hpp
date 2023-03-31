#ifndef JINJA_HPP
#define JINJA_HPP
#include <string>
#include <templates/inja.hpp>
#include <json/json.hpp>

namespace Template {
    class Jinja {
        public:
            std::string render(std::string templateName, JSON data);
            Jinja(std::string templateDir);
            inja::Environment env;
        private:
            std::string templateDir;
    };
}
#endif

