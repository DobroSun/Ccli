#include "ccli/HeaderSearch.hpp"
#include "ccli/Utility.hpp"

#include <cstring>
#include <deque>
#include <dirent.h>


#define GET_HEADERS_CMD "gcc -v -E -xc - < /dev/null 2>&1 | sed -ne '/starts here/,/End of/p' | grep -v '#include' | grep -v 'End of search list'"


static auto get_cmd_headers = [] {
    return get_splitted_exec(GET_HEADERS_CMD);
};

static auto ltrim_headers = [&](std::vector<std::string> &vec) {
    for(unsigned i = 0; i < vec.size(); i++) {
        vec[i] = ltrim(vec[i]);
    }
    return vec;
};


static auto extend = [&](std::vector<std::string> &dest, std::vector<std::string> &source) {
    for(std::string el: source) {
        dest.push_back(el);
    }
};


static auto process_headers = [&](std::vector<std::string> &headers) {
    for(unsigned i = 0; i < headers.size(); i++) {
        headers[i] = add_I_option(headers[i]) + "/";
    }
};


void put_subdirectories(std::vector<std::string> &vec, std::string &path) {
    std::deque<std::string> deque;
    deque.push_back(path);

    while(!deque.empty()) {
        std::string dir_path = deque.front();
        deque.pop_front();

        DIR *dir = opendir(dir_path.c_str());
        struct dirent *entry = readdir(dir);
        while(entry) {
            if(entry->d_type == DT_DIR) {
                if(strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {
                    std::string subdir_path = dir_path+"/"+entry->d_name;

                    vec.push_back(subdir_path);
                    deque.push_back(subdir_path);


                    debug() << subdir_path << " <- pushed to subdirs" << std::endl;
                }
            }
            entry = readdir(dir);
        }
        closedir(dir);
    }
}


std::vector<std::string> get_headers() {
    std::vector<std::string> headers;
    
    headers = get_cmd_headers();
    headers = ltrim_headers(headers);

	std::vector<std::string> subdirectories;
    for(std::string path: headers) {
        put_subdirectories(subdirectories, path);
    }
    extend(headers, subdirectories);

    process_headers(headers);
    return headers;
};

