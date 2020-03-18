#include "ccli/history.hpp"
#include "ccli/context.hpp"

#include <iostream>
#include <cassert>

namespace ccli {

void history::add_history(const context *cxt) {
  ht.push_back(new context(*cxt));
}

void history::clear_history(unsigned ind) {
  assert(ind <= ht.size() && "Index Error: cannot use unexisting index");
  for(; ind < ht.size(); ind++) {
    context *cc = ht.back();

    delete cc;
    ht.pop_back();
  }
}

context *history::get_history(unsigned ind) {
  if(ind > ht.size()) {
    std::cout << "Nothing to undo!" << std::endl;
    return new context();
  }
  context *tmp = new context(*ht[ht.size()-ind]);
  clear_history(ht.size()-ind);
  return tmp;
}

history::~history() {
  for(auto &each: ht) {
    delete each;
  }
}

}; // namespace ccli
