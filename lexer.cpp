#include "lexer.h"
#include "utility.h"
#include "value.h"

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
static auto is_floating_point_number(const char *c) -> bool {
  while(isdigit(*c)) {
    assert(*c);
    c++;
  }
  if(*c == '.') {
    return true;
  } else {
    return false;
  }
}

static auto set_number_token() -> void {
  current_token.type = LiteralTok;
  if(is_floating_point_number(cursor)) {
    current_token.val.tag = F32;
    current_token.val.float_value = atof(cursor);
  } else {
    current_token.val.tag = I32;
    current_token.val.integer_value = atoi(cursor);
  }
}

static auto increment_cursor_on_number() -> void {
  while(isdigit(*cursor)) {
    inc_cursor();
  }
  if(*cursor == '.') {
    inc_cursor();

    while(isdigit(*cursor)) {
      inc_cursor();
    }

    if(*cursor == 'f') {
      inc_cursor();
    }
  }
}

static auto process_number() -> void {
  set_number_token();
  increment_cursor_on_number();
}
//

// CharTok.
static auto set_char_token() -> void {
  current_token.type = LiteralTok;

  inc_cursor();
  current_token.val.tag = U8;
  current_token.val.char_value = *cursor;
}

static auto increment_cursor_on_character() -> void {
  inc_cursor();
  assert(*cursor == '\'');
  inc_cursor();
}

static auto process_character() -> void {
  set_char_token();
  increment_cursor_on_character();
}

// Symbols.
static auto set_sym_token() -> void {
  current_token.type = (TokenType)*cursor;
  current_token.identifier_value.string = cursor;
  current_token.identifier_value.size   = 1;
}

static auto process_symbol() -> void {
  set_sym_token();
  inc_cursor();
}
//

// IdentifierTok.

// @Incomplete:
// Handle numbers inside an identifier.
static auto is_identifier(char c) -> bool {
  return isalpha(c) || c == '_';
}

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

// `//`.
static auto process_single_line_comment() -> void {
  while(*cursor != '\n') {
    if(*cursor == '\0') return;
    assert(*cursor);
    inc_cursor();
  }
  inc_cursor();
}
// 

// `/*`.
static auto process_multi_line_comment(u8 count=1) -> void {
  inc_cursor();
  while(count) {
    while(*cursor != '*') {
      if(*cursor == '\0') return;

      if(*cursor == '/') {
        inc_cursor();
        if(*cursor == '*') {
          count++;
        }
      }
      inc_cursor();
    }
    assert(*cursor == '*');
    inc_cursor();
    if(*cursor == '/') {
      inc_cursor();
      count--;

    } else {
      process_multi_line_comment(count);
    }
  }
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
//

// @SpeedUp: Don't need to lex again if tok has been already peeked.
auto next_token() -> Token {
  while(*cursor) {
    if(isdigit(*cursor) || *cursor == '.') {
      process_number();

    } else if(*cursor == '/') {
      inc_cursor();
      if(*cursor == '/') {
        // one line comment.
        process_single_line_comment();
        continue;

      } else if(*cursor == '*') {
        // multi line comment.
        process_multi_line_comment();
        continue;

      } else {
        cursor--;
        process_symbol();
      }

    } else if(*cursor == '\'') {
      // 'c', 'a', '0' single characters..
      process_character();

    } else if(in(*cursor, "+-*()?:;")) {
      process_symbol();

    } else if(is_identifier(*cursor)) {
      // idents.
      process_identifier();

    } else {
      // ` `, `\t`, etc.
      inc_cursor();
      continue;
    }
    return current_token;
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
