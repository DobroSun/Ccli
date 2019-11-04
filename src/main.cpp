#include <iostream>
#include <string>
#include <csignal>
#include <queue>
#include <mutex>
#include <fstream>
#include <cstdio>
#include <stdexcept>
#include <memory>
#include <cstring>
#include <array>
#include <list>
#include <readline/readline.h>
#include <readline/history.h>

#include "ccli/Utils.hpp"

#define TMP_FILE "../tmp/tmp.cpp"
#define TMP_DIR "../tmp/"


template<typename T>
class th_safe_queue {
    std::mutex _lock;
    std::queue<T> elements;
public: 
    bool empty() {
        return this->elements.empty();
    }
    void push(T&& value) {
        std::lock_guard<std::mutex> lock(this->_lock);
        this->elements.push(value);
    }
    T get() {
        T value = this->elements.front();
        while (this->elements.empty());
        std::lock_guard<std::mutex> lock(this->_lock);
        this->elements.pop();
        return value;
    }
    T get_nowait() {
        T value = this->elements.front();
        std::lock_guard<std::mutex> lock(this->_lock);
        this->elements.pop();
        return value;
    }
};

std::string welcome() {
    std::string welcome("Hello world");
    welcome += "> ";
    return welcome;
}

void load_file(char *filename) {
    std::cout << "Loading " << filename << "\n";
}

void close_all(std::ofstream *tmp_file) {
    tmp_file->close();
}

void write_cmds_to_file(std::ofstream &tmp_file, std::string cmd="", std::string pre_cmd="") {
	tmp_file << "#include <iostream>\n";
    tmp_file << pre_cmd << "\n";
    tmp_file << "int main() {\n";
	tmp_file << "std::cout << ";
    tmp_file << cmd << " << std::endl;";
    tmp_file << "\n";
    tmp_file << "return 0;\n";
    tmp_file << "}";
	tmp_file.flush();
}

std::string compile_and_run(std::ofstream &tmp_file) {
	std::string cmd("cd ");
    cmd += TMP_DIR; cmd += ";g++ "; cmd += TMP_FILE; cmd += " && "; cmd += "./a.out";

    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("Compilation error");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != NULL) {
        result += buffer.data();
    }
    return result;
}

int main(int argc, char* argv[]) {
    History history = History();

    if (argc > 1) {
        load_file(argv[1]);
    }
    
    std::ofstream tmp_file(TMP_FILE);
    if (!tmp_file) {
        std::cout << "No tmp_file!\n";
    }

	// handle Ctrl-C interruption
    struct sigaction act;
    act.sa_handler = [](int sig){
                        std::cout << "\n";
                        rl_on_new_line();
                        rl_replace_line("", 0);
                        rl_redisplay();
                     };
    sigaction(SIGINT, &act, NULL);
    
	while (1) {
        const char *line = readline(welcome().c_str());

        if (line == NULL) {std::cout << "\n"; close_all(&tmp_file); exit(0);}
        if (!std::strcmp(line, "")) continue;


        // Shit
		std::remove(TMP_FILE);
		std::ofstream tmp_file(TMP_FILE);

        add_history(line);
        //write_cmds_to_file
        write_cmds_to_file(tmp_file, line);
        std::string result = compile_and_run(tmp_file);

        std::cout << result;
    }
    return 0;
}
