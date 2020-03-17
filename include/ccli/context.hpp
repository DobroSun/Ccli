#ifndef CCLI_INCLUDE_CCLI_CONTEXT_HPP
#define CCLI_INCLUDE_CCLI_CONTEXT_HPP
#include <string>
#include <vector>

namespace ccli {

enum class push_to {
  decls,
  mains,
  includes,
};

class context {
	std::vector<std::string> main_commands;
	std::vector<std::string> decl_commands;
	std::vector<std::string> includes;

	std::vector<std::string> _main;
	std::vector<std::string> _return_main;
	std::vector<std::string> _include;
public:
	context();

  void add_main_cmd(const std::string &cmd);
  void add_decl_cmd(const std::string &cmd);
  void add_include_cmd(const std::string &cmd);

  void clear_mains();
  void clear_decls();
  void clear_includes();

  std::vector<std::string> get_includes();
	std::string get_content();
};
}
#endif
