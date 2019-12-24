#ifndef UTILITY_CPP
#define UTILITY_CPP
#include <vector>
#include <string>

std::string join(const std::vector<std::string> &context, char ch='\n');
std::string join(std::vector<std::string> &&context, char ch='\n');

std::vector<std::string> split(const std::string &str, char ch='\n');
std::vector<std::string> split(std::string &&str, char ch='\n');

std::string exec(const std::string &cmd);
std::string exec(std::string &&cmd);

std::vector<std::string> get_splitted_exec(const std::string &cmd, char ch='\n');
std::vector<std::string> get_splitted_exec(std::string &&cmd, char ch='\n');

std::string ltrim(std::string &str);
std::string rtrim(std::string &str);
std::string trim(std::string &str);


std::string add_I_option(const std::string &dest);
std::string add_I_option(std::string &&dest);

template<typename T, typename A>
A map(T func, const A &vec);
template<typename T, typename A>
A map(T func, A &&vec);


template<typename T>
void print(const std::vector<T> &vec);
template<typename T>
void print(std::vector<T> &&vec);


#include "ccli/all_template_implementations.hpp"
#endif
