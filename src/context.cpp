#include "ccli/context.hpp"
#include "ccli/utility.hpp"

using ccli::utility::join;

namespace ccli {

context::context() {
	_main = {
					"int main() {",
					};
	 _return_main = {
					"return 0;",
					"}",
					};
	_include = {
					"#include <iostream>",
					};
}

void context::clear_mains() {
  main_commands.clear();
}
void context::clear_decls() {
  decl_commands.clear();
}
void context::clear_includes() {
  includes.clear();
}

void context::add_main_cmd(const std::string &cmd) {
  main_commands.push_back(cmd);
}

void context::add_decl_cmd(const std::string &cmd) {
  decl_commands.push_back(cmd);
}

void context::add_include_cmd(const std::string &cmd) {
  includes.push_back(cmd);
}

std::vector<std::string> context::get_includes() {
  return includes;
}

std::string context::get_content() {
  std::string file_buffer;
  file_buffer += join(_include);
  file_buffer += join(includes);
  file_buffer += join(decl_commands);
  file_buffer += join(_main);
  file_buffer += join(main_commands);
  file_buffer += join(_return_main);
	return file_buffer;
}
} // namespace
