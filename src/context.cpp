#include "ccli/context.hpp"

context::context() {
	main_commands = {""};
	decl_commands = {""};

	_main = {
					"int main() {",
					};
	 _return_main = {
					"return 0;",
					"}",
					};
	_include = {
					"#include <iostream>",
					//"#include <string>",
					};
}

void context::add_command(command &vv) {
	vv->push();
}
