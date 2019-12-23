#include <vector>
#include <string>
std::vector<std::string> get_headers();

// FIXME:
// Try to rewrite this as function.
// It's impossible to return vector<const char*> from function,
// or change it by reference.
// So for now it's macro.
#define make_headers(source, vec) [&](const std::vector<std::string> &ptr, std::vector<const char*> &vec) { \
        for(unsigned i = 0; i < source.size(); i++) { \
            vec.push_back(source[i].c_str()); \
        } \
    }(source, vec)
