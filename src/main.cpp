#include <iostream>
#include <functional>
#include <string>
#include <csignal>
#include <queue>
#include <mutex>
#include <thread>
#include <fstream>


class StructWriter;
class CommandWriter;

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

class IManager {
    std::ofstream *tmp_file;
    StructWriter s_writer;
public:
    IManager(std::ofstream *tmp_file) {
        this->tmp_file = tmp_file;
        this->s_writer = new StructWriter();
        this->c_writer = new CommandWriter();
    }
    bool write_to_file(std::string *line) {
        if (line[0] == ":") {
            std::cout << "Can't handler this yet\n";
        } else if (line[0] == "|") {
            return s_writer.write(line);
        } else {
            return c_writer.write(line);
        }
    }
};

class Executor {
    th_safe_queue<bool> *bool_q;
    std::ofstream *tmp_file;
public:
    Executor(th_safe_queue<bool> *q) {
        this->bool_q = q;
    }
    void operator()() {
        // compile first time
        while (1) {
            if (!this->bool_q->get()) std::cout << "Exception in Handler\n";
            this->compile();
            
            std::string res = this->execute();
        }
    }
    void compile() {
        //
    }
    std::string execute() {
    }
};

class Handler {
    th_safe_queue<std::string *> *from_main;
    th_safe_queue<std::string *> *to_main;
    th_safe_queue<bool> *bool_q;
    std::ofstream *tmp_file;
    std::string *cmd;

    IManager *i_manager;
public:
    Handler(auto *from_main_q, auto *to_main_q, auto *b_q, std::ofstream *tmp_file) {
        this->from_main = from_main_q;
        this->to_main = to_main_q;
        this->bool_q = b_q;
        this->tmp_file = tmp_file;

        this->i_manager = new IManager(tmp_file);
    }
    ~Handler() {
        delete this->i_manager;
    }
    void operator()() {
        while (1) {
            this->get_from_q();

            bool is_success = i_manager->write_to_file(this->cmd);
            this->bool_q->push(&is_success);
        }
    }
    void get_from_q() {
        this->cmd = this->from_main->get();
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

void init_file(std::ofstream *tmp_file) {
    *tmp_file << "int main() {\n";
    *tmp_file << "\n";
    *tmp_file << "return 0;";
    *tmp_file << "}";
}

void close_all() {
    //join threads
}

int main(int argc, char* argv[]) {
    if (argc > 1) {
        load_file(argv[1]);
    }
    std::string line;
    th_safe_queue<std::string *> send_q;
    th_safe_queue<std::string *> get_q; 
    th_safe_queue<bool> access_q; 
    
    std::ofstream tmp_file("exec.cpp");
    init_file(&tmp_file);

    Handler handler = Handler(&send_q, &get_q, &access_q, &tmp_file);

    std::thread t1(handler);

	// handle Ctrl-C interruption
    struct sigaction act;
    act.sa_handler = handle_ctrl_c;
    sigaction(SIGINT, &act, NULL);
    
	while (1) {
        std::cout << welcome() << ">";

        std::getline(std::cin, line);
        if (is_interrupted) {is_interrupted = false; std::cin.clear(); continue;}
        if (std::cin.eof()) {std::cout << "\n"; close_all(); exit(0);}
        if (line.empty()) continue;

        send_on_handling(send_q, line);

        std::string *res = get_result(get_q);

        std::cout << *res << "\n";
    }
    t1.join();
    return 0;
}
