#include "ccli/Loader.hpp"

#define TMP_FILE "../tmp/tmp.cpp"
#define TMP_DIR "../tmp/"

Loader::Loader() {
}

void Loader::load_file(std::string filename) {
    std::ifstream file(filename);
    std::ofstream tmp_file(TMP_FILE);   
    
    std::string line;
    while (getline(file, line)) {
        tmp_file << line << "\n";  
    }
    tmp_file.flush();
    tmp_file.close();
}

