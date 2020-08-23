#ifndef CCLI_GLOBALS_H
#define CCLI_GLOBALS_H
#include <cinttypes>
#include <cstdio>

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;

using f32 = float;
using f64 = double;


// @Note:
// Strings are correct only if entire file is loaded into memory.
struct ident_string {
  const char *string;
  u8          size;
};

void inline print_string(const ident_string &s) {
  const char *tmp = s.string;
  for(u8 i = 0; i < s.size; i++) {
    printf("%c", *tmp);
    tmp++;
  }
}

#endif
