#ifndef UTILITY_CPP
#define UTILITY_CPP
#include <vector>
#include <string>

#define make_decorator(decorator, func) \
    decorator<decltype(func)>(func)

namespace ccli {
namespace utility {
std::string join(const std::vector<std::string> &context, char ch='\n');
std::vector<std::string> split(const std::string &str, char ch='\n');

std::string exec(const std::string &cmd);

std::vector<std::string> get_splitted_exec(const std::string &cmd, char ch='\n');

std::string ltrim(std::string &str);
std::string rtrim(std::string &str);
std::string trim(std::string &str);
bool equals(const std::string &s1, const std::string &s2);
bool ends_with(std::string &cmd, const std::string &ch);
bool starts_with(std::string &cmd, const std::string &ch);
bool find(std::string &cmd, const std::string &ch);
int find_place(std::string &cmd, const std::string &ch);


template<typename T>
auto silent(T func);

template<typename T>
auto no_errors(T func);

template<typename T>
auto get_output(T func);

template<class T, class, class>
struct is_mapping;
template<class T>
constexpr bool is_mapping_v;

template<class T, class>
struct is_iterable;
template<class T>
constexpr bool is_iterable_v;

template<class T, class F>
void map(T&& m, F f);

template<typename T>
void print(const std::vector<T> &vec);

}; // namespace utility
}; // namespace ccli
#include "ccli/all_template_implementations.hpp"
#endif
