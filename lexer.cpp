#include "lexer.h"
#include "utility.h"

#include <cctype>
#include <cstdlib>
#include <cstdio>
#include <cassert>


static Token current_token;
static const char *cursor;

static auto inc_cursor() -> void {
  assert(*cursor);
  cursor++;
}

// LiteralTok.
static auto set_number_token() -> void {
  current_token.type = LiteralTok;
  current_token.integer_value = atoi(cursor);
}

static auto increment_cursor_on_number() -> void {
  inc_cursor();
  while(isdigit(*cursor)) {
    inc_cursor();
  }
}

static auto process_number() -> void {
  set_number_token();
  increment_cursor_on_number();
}
//

// Characters.
static auto set_char_token() -> void {
  current_token.type = (TokenType)*cursor;
  current_token.identifier_value.string = cursor;
  current_token.identifier_value.size   = 1;
}

static auto process_character() -> void {
  set_char_token();
  inc_cursor();
}
//


// @Incomplete:
// Handle numbers inside an identifier.
static auto is_identifier(char c) -> bool {
  return isalpha(c) || c == '_';
}

// Identifier.
static auto set_identifier_token() -> void {
  current_token.type = IdentifierTok;
  current_token.identifier_value.string = cursor;

  i32 count = 0;
  while(is_identifier(*cursor)) { count++; inc_cursor(); }
  current_token.identifier_value.size = count;
}

static auto process_identifier() -> void {
  set_identifier_token();
}
//

// End tok.
static auto set_end_token() -> void {
  current_token.type = EndTok;
}

static auto end_input() -> void {
  cursor = nullptr;
}

static auto process_end_token() -> void {
  set_end_token();
  end_input();
}
// .

// @SpeedUp: Don't need to lex again if tok has been already peeked.
auto next_token() -> Token {
  while(*cursor) {
    if(isdigit(*cursor)) {
      process_number();
      return current_token;

    } else if(in(*cursor, "+-*/()?:;")) {
      process_character();
      return current_token;

    } else if(is_identifier(*cursor)) {
      process_identifier();
      return current_token;

    } else {
      inc_cursor();
    }
  }
  process_end_token();
  return current_token;
}

auto peek_token() -> Token {
  const char *tmp = cursor;
  Token tok = next_token();
  cursor = tmp;
  return tok;
}

auto set_cursor(const char *input) -> void {
  cursor = input;
}
