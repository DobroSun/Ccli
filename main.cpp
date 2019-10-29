#include <iostream>
#include <string>
#include <csignal>

static bool is_interrupted = false;

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

int main(int argc, char* argv[]) {
    if (argc > 1) {
        load_file(argv[1]);
    }
    std::string line;

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

        
        std::cout << line << "\n";
    }
    return 0;
}
