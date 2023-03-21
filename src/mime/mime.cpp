#include <string>
#include <mime/mime.hpp>
#include <mime/mime-lookup.def>

std::string getMimeType(std::string filename){
    std::string extension = filename.substr(filename.find_last_of(".") + 1);
    
    std::string type = std::string(Perfect_Hash::lookup_mime(extension.c_str(), extension.length()) -> mime); 

    if(type.empty()){
        return "text/plain";
    }
    return type;
}

