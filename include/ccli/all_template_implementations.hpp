#ifndef ALL_TEMPLATE_IMPLEMENTATIONS
#define ALL_TEMPLATE_IMPLEMENTATIONS
#include <type_traits>
#include <algorithm>
#include <iostream>
#include <array>
#include <vector>

#include <unistd.h>


#include "ccli/logger.hpp"

namespace ccli {
namespace utility {

// Prints all items from given collection.
template<typename T>
void print(const std::vector<T> &vec) {
    debug() << "[";
    for(unsigned int i = 0; i < vec.size(); i++) {
        bool pr = false;
        bool cm = false;
        if constexpr (std::is_same<T, std::string>::value ||
                      std::is_same<T, const char*>::value) {
            debug() << "\"";
            pr = true;
        }
        if((vec.size() - i) > 1) {
            cm = true;
        }
        debug() << vec[i];
        if(pr) debug() << "\"";
        if(cm) debug() << ", ";
    }
    debug() << "]\n";
}


// Executes given function silently.
// FIXME:
// Cannot handle function with default arguments.
// Requires complete argument list.
template<typename T>
auto silent(T func) {
    auto r_func = [=](auto ...args) {
        int out_pipe[2];
        int saved_stdout, saved_stderr;

        saved_stdout = dup(STDOUT_FILENO);
        saved_stderr = dup(STDERR_FILENO);
        if(pipe(out_pipe) != 0) {
            std::cout << "Cannot open pipe in decorator!" << std::endl;
            exit(1);
        }

        dup2(out_pipe[1], STDOUT_FILENO);
        dup2(out_pipe[1], STDERR_FILENO);
        auto res = func(args...);

        close(out_pipe[1]);
        dup2(saved_stdout, STDOUT_FILENO);
        dup2(saved_stderr, STDERR_FILENO);
        return res;
    };
    return r_func;
}

template<typename T>
auto no_errors(T func) {
    auto r_func = [=](auto ...args) {
        int out_pipe[2];
        int saved_stderr;

        saved_stderr = dup(STDERR_FILENO);
        if(pipe(out_pipe) != 0) {
            std::cout << "Cannot open pipe in decorator!" << std::endl;
            exit(1);
        }

        dup2(out_pipe[1], STDERR_FILENO);
        auto res = func(args...);

        close(out_pipe[1]);
        dup2(saved_stderr, STDERR_FILENO);
        return res;
    };
    return r_func;
}

// Executes given function,
// Returns all output send to stdout
// While processing function.

// Does not work as expected.
// Returns needed string with ovehead.
template<typename T>
auto get_output(T func) {
    auto r_func = [=](auto... args) {
        std::array<char, 128> buffer;
        int out_pipe[2];
        int saved_stdout;

        saved_stdout = dup(STDOUT_FILENO);
        if(pipe(out_pipe) != 0) {
            std::cout << "Cannot open pipe in decorator!" << std::endl;
            exit(1);
        }

        dup2(out_pipe[1], STDOUT_FILENO);
        func(args...);

        fflush(stdout);
        close(out_pipe[1]);
        dup2(saved_stdout, STDOUT_FILENO);

        std::string output;
        while(read(out_pipe[0], buffer.data(), buffer.size()) != 0) {
            output += buffer.data();
        }
        return output;
    };
    return r_func;
}

template<class T, class = void>
struct is_iterable: std::false_type {};

template<class T>
struct is_iterable
  <T, std::void_t<typename T::iterator>>
  : std::true_type {};

template<class T>
constexpr bool is_iterable_v = is_iterable<T>::value;


template
    <
    class T, class = std::void_t<>, class = std::void_t<>
    >
struct is_mapping
    : std::false_type {};

template<class T>
struct is_mapping
    <T,
     std::void_t<typename T::key_type>,
     std::void_t<typename T::mapped_type>
    >   
    : std::true_type {};

template<class T>
constexpr bool is_mapping_v = is_mapping<T>::value;

template<class T, class F, class = void>
class map__;

template<class T, class F>
class map__<T, F, std::enable_if_t<is_iterable_v<T>>> {
  void handle_maps(const T &m, F f, std::true_type) {
    std::cout << "RUNNING ON MAP CONTAINER" << std::endl;
    T &tmp = const_cast<T&>(m);
    if constexpr(std::is_same<
                  std::result_of_t
                  <
                    decltype((f))(std::add_lvalue_reference_t<typename T::value_type>)
                  >,
                  void
                  >::value) {
      std::cout << "Calling function returning void on map!" << std::endl;
      std::for_each(tmp.begin(), tmp.end(), f);
    } else  {
      std::cout << "Calling function returning value on map!" << std::endl;
      for(auto const &[key, value]: m) {
        auto res = f(std::make_pair(key, value));
        tmp[res.first] = res.second;
      }
    }
  }
  void handle_maps(const T &m, F f, std::false_type) {
    T &tmp = const_cast<T&>(m);
    if constexpr(std::is_same<
                  std::result_of_t
                  <
                    decltype((f))(std::add_lvalue_reference_t<typename T::value_type>)
                  >,
                  void
                  >::value) {
      std::for_each(tmp.begin(), tmp.end(), f);
    } else {
      std::transform(m.begin(), m.end(), tmp.begin(), f);
    }
  }
public:
  void operator()(const T& m, F f) {
    handle_maps(m, f, std::integral_constant<bool, is_mapping_v<T>>());
  }
};

template<class T, class F>
void map(T&& m, F f) {
  map__<std::remove_reference_t<T>, F> M;
  return M(std::forward<T>(m), f);
}
}; // namespace utility
}; // namespace ccli
#endif
