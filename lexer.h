#ifndef CCLI_LEXER_H
#define CCLI_LEXER_H
#include "globals.h"

enum TokenType {
  IdentifierTok = 256,
  LiteralTok    = 257,
  ErrorTok      = 258,
  EndTok        = 259,
};

struct Token {
  TokenType type = ErrorTok;

  union {
    u8           char_value;
    i32          integer_value; // @Incomplete: How do I handle unsigned?
    u32          unsigned_value;
    f32          float32_value;
    f64          float64_value;
    ident_string identifier_value;
  };
};


auto next_token() -> Token;
auto peek_token() -> Token;
auto set_cursor(const char *) -> void;

#endif
