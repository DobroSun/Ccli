#ifndef GAMMA_HISTORY_HPP
#define GAMMA_HISTORY_HPP
#include <vector>

namespace ccli {

class context;
class history {
  std::vector<context*> ht;
  void clear_history(unsigned ind);
public:
  ~history();
  void add_history(const context *cxt);
  context *get_history(unsigned ind);
};

}; // namepsace ccli
#endif
