#ifndef CCLI_UTILITY_H
#define CCLI_UTILITY_H

template<class T, class U>
bool in(T v, const U *array) {
  auto it = array;
  while(*it) {
    if(static_cast<U>(v) == *it) {
      return true;
    }
    ++it;
  }
  return false;
}

#endif
