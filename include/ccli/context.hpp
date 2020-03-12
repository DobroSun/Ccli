#ifndef CCLI_INCLUDE_CCLI_CONTEXT_HPP
#define CCLI_INCLUDE_CCLI_CONTEXT_HPP
#include <string>
#include <vector>

namespace ccli {
class command;
class context {
	std::vector<std::string> main_commands;
	std::vector<std::string> decl_commands;

	std::vector<std::string> _main;
	std::vector<std::string> _return_main;
	std::vector<std::string> _include;
public:
	context();

	void add_command(command &cmd);
	std::string get_context();
};
}
#endif
