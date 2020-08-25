#ifndef CCLI_LEXER_H
#define CCLI_LEXER_H
#include "globals.h"
#include "value.h"

enum TokenType {
  IdentifierTok = 256,
  LiteralTok    = 257,
  ErrorTok      = 258,
  EndTok        = 259,
};

struct Token {
  TokenType type = ErrorTok;

  union {
    Value val;
    ident_string identifier_value;
  };
};


auto next_token() -> Token;
auto peek_token() -> Token;
auto set_cursor(const char *) -> void;

#endif
