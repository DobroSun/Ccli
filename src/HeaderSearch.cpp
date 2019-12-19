#include "ccli/HeaderSearch.hpp"
#include "ccli/Utility.hpp"

#include <cstring>
#include <stack>
#include <dirent.h>


#define GET_HEADERS_CMD "gcc -v -E -xc - < /dev/null 2>&1 | sed -ne '/starts here/,/End of/p' | grep -v '#include' | grep -v 'End of search list'"
    

static auto get_cmd_headers = [] {
    return get_splitted_exec(GET_HEADERS_CMD);
};

static auto ltrim_headers = [&](std::vector<std::string> &vec) {
    for(unsigned int i = 0; i < vec.size(); i++) {
        vec[i] = ltrim(vec[i]);
    }
    return vec;
};


static auto add_I_opt = [&](std::vector<std::string> &vec) {
    for(unsigned int i = 0; i < vec.size(); i++) {
        vec[i] = add_I_option(vec[i]);
    }
    return vec;
};

static auto make_chared = [&](std::vector<std::string> &vec) {
    std::vector<const char*> res(vec.size(), nullptr);
    for(unsigned int i = 0; i < vec.size(); i++) {
        res[i] = vec[i].c_str();
    }
    return res;
};


static auto extend = [&](std::vector<std::string> &dest, std::vector<std::string> &source) {
    for(std::string el: source) {
        dest.push_back(el);
    }
};


void put_subdirectories(std::vector<std::string> &vec, std::string &path) {
    DIR *dir = opendir(path.c_str());
    debug() << path.c_str() << " <- Opened" << std::endl;
    struct dirent *entry = readdir(dir);

    debug() << "Before while loop" << std::endl;
    while(entry) {
        if(entry->d_type == DT_DIR) {
			if(strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {
				vec.push_back(path);
                debug() << path << " <- pushed to subdirs" << std::endl;
            }
            //if(entry) delete entry;
            std::string sub_path = path + entry->d_name;
            put_subdirectories(vec, sub_path);

		}
        entry = readdir(dir);
    }
    closedir(dir);
}



// TODO: Rewrite all with functions and test it.
std::vector<const char*> get_headers() {
    std::vector<const char*> result;
    std::vector<std::string> headers;
    
    headers = get_cmd_headers();

    headers = ltrim_headers(headers);
/*
	std::vector<std::string> subdirectories;
    for(std::string path: headers) {
        put_subdirectories(subdirectories, path);
    }
    extend(headers, subdirectories);
*/

    headers = add_I_opt(headers);

    debug() << "Processed headers: " << std::endl;
    result = make_chared(headers);
    print(result);
    return result;
};





