#include <iostream>
#include <string>
#include <csignal>
#include <queue>
#include <mutex>

static bool is_interrupted = false;

template<typename T>
class th_safe_queue {
    std::mutex _lock;
    std::queue<T> elements;
public: 
    bool empty() {
        elements.empty();
    }
    void push(T&& value) {
        std::lock_guard<std::mutex> lock(_lock);
        elements.push(value);
    }
    T get() {
        T value = elements.front();
        std::lock_guard<std::mutex> lock(_lock);
        elements.pop();
        return value;
    }
};
class Handler {
    th_safe_queue<std::string *> *queue;
    std::string *cmd;
public:
    Handler(th_safe_queue<std::string *> *q) {
        queue = q;
    }
    void operator()() {
        while (1) {
            get_from_q();
                   
            analise_cmd();
        }
    }

    void get_from_q() {
        while (queue->empty());
        cmd = queue->get();
    }

    void analise_cmd() {
    }
};

std::string welcome() {
    std::string welcome("Hello world");
    return welcome;
}

void load_file(char *filename) {
    std::cout << "Loading " << filename << std::endl;
}

void handle_ctrl_c(int sig) {
    std::cout << "" << std::endl;
    is_interrupted = true;
}

void send_on_handling(th_safe_queue<std::string *> &send_q, std::string &line) {
    send_q.push(&line);
}

std::string *get_result(th_safe_queue<std::string *> &get_q) {
    while (get_q.empty());
    return get_q.get();
}

int main(int argc, char* argv[]) {
    if (argc > 1) {
        load_file(argv[1]);
    }
    std::string line;
    th_safe_queue<std::string *> send_q;
    th_safe_queue<std::string *> get_q; 
    th_safe_queue<bool> access_q; 

    Handler handler = Handler(&send_q);
    // And temporary file!


	// handle Ctrl-C interruption
    struct sigaction act;
    act.sa_handler = handle_ctrl_c;
    sigaction(SIGINT, &act, NULL);
    
	while (1) {
        std::cout << welcome() << ">";

        std::getline(std::cin, line);
        if (is_interrupted) {is_interrupted = false; std::cin.clear(); continue;}
        if (std::cin.eof()) {std::cout << "\n"; exit(0);}
        if (line.empty()) continue;

        send_on_handling(send_q, line);

        std::string *res = get_result(get_q);

        std::cout << *res << "\n";
    }
    return 0;
}
