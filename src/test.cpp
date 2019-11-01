#include <iostream>
#include <string>
#include <csignal>
#include <queue>
#include <mutex>
#include <fstream>
#include <cstdio>
#include <stdexcept>
#include <memory>
#include <array>

static bool is_interrupted = false;

template<typename T>
class th_safe_queue {
    std::mutex _lock;
    std::queue<T> elements;
public: 
    bool empty() {
        this->elements.empty();
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
    return welcome;
}

void load_file(char *filename) {
    std::cout << "Loading " << filename << "\n";
}

void handle_ctrl_c(int sig) {
    std::cout << "\n";
    is_interrupted = true;
}

void send_on_handling(th_safe_queue<std::string *> &send_q, std::string &line) {
    send_q.push(&line);
}

std::string *get_result(th_safe_queue<std::string *> &get_q) {
    return get_q.get();
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
	const char *cmd = "g++ tmp.cpp && ./a.out";
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

int main(int argc, char* argv[]) {
    if (argc > 1) {
        load_file(argv[1]);
    }
    std::string line;
    
    std::ofstream tmp_file("tmp.cpp");
    if (!tmp_file) {
        std::cout << "Not tmp_file!\n";
    }

	// handle Ctrl-C interruption
    struct sigaction act;
    act.sa_handler = handle_ctrl_c;
    sigaction(SIGINT, &act, NULL);
    
	while (1) {
        std::cout << welcome() << ">";

        std::getline(std::cin, line);
        if (is_interrupted) {is_interrupted = false; std::cin.clear(); continue;}
        if (std::cin.eof()) {std::cout << "\n"; close_all(&tmp_file); exit(0);}
        if (line.empty()) continue;

        //write_cmds_to_file
		std::remove("tmp.cpp");
		std::ofstream tmp_file("tmp.cpp");

        write_cmds_to_file(tmp_file, line);
        std::string result = compile_and_run(tmp_file);

        std::cout << result;
    }
    return 0;
}
