#include "lexer.h"
#include "utility.h"

#include <cassert>
#include <csignal>
#include <cstdlib>
#include <memory>


#if 0
#include <iostream>
#endif

struct Ast_Expression;
static auto destruct(Ast_Expression*) -> void;


template<class T>
using just_unique = std::unique_ptr<T, decltype(&destruct)>;

template<class T>
struct ast_ptr: public just_unique<T> {
  using just_unique<T>::unique_ptr;

  ast_ptr(T *p=nullptr): just_unique<T>{p, destruct} {}
};

template<class T, class... Args>
auto make_ast(Args&& ...args) -> ast_ptr<T> {
  return ast_ptr<T>(new T(std::forward<Args>(args)...));
}


static auto is_binary_operator(TokenType c) -> bool { 
  return static_cast<i32>(c) < 256 && in(c, "+-*/?:");
         
}
static auto is_unary_operator(TokenType c) -> bool { 
  return static_cast<i32>(c) < 256 && in(c, "+-");
}

enum ValueTag {
  I32, // int
  U8,  // char
  U32, // unsigned
  F32, // float
  F64  // double
};

struct Value {
  i32 integer_value;
};



enum Ast_Type {
  Ast_Variable_Type,
  Ast_Literal_Type,
  Ast_AddOperator_Type,
  Ast_SubOperator_Type,
  Ast_MulOperator_Type,
  Ast_DivOperator_Type,
  Ast_UnaryMinus_Type,
  Ast_TernaryOperator_Type,
};

struct Ast_Expression {
  Ast_Type ast_type;

  Ast_Expression() = delete;
  Ast_Expression(Ast_Type type): ast_type{type} {}
};

struct Ast_Variable: public Ast_Expression {
  using Ast_Expression::Ast_Expression;

  ast_ptr<Ast_Expression> expr;
  ident_string type;
  ident_string name;
};

struct Ast_Literal: public Ast_Expression {
  using Ast_Expression::Ast_Expression;

  Value value;
};

struct Ast_AddOperator: public Ast_Expression {
  using Ast_Expression::Ast_Expression;

  ast_ptr<Ast_Expression> left;
  ast_ptr<Ast_Expression> right;
};

struct Ast_SubOperator: public Ast_Expression {
  using Ast_Expression::Ast_Expression;

  ast_ptr<Ast_Expression> left;
  ast_ptr<Ast_Expression> right;
};

struct Ast_MulOperator: public Ast_Expression {
  using Ast_Expression::Ast_Expression;

  ast_ptr<Ast_Expression> left;
  ast_ptr<Ast_Expression> right;
};

struct Ast_DivOperator: public Ast_Expression {
  using Ast_Expression::Ast_Expression;

  ast_ptr<Ast_Expression> left;
  ast_ptr<Ast_Expression> right;
};

struct Ast_UnaryMinus: public Ast_Expression {
  using Ast_Expression::Ast_Expression;

  ast_ptr<Ast_Expression> operand;
};

struct Ast_TernaryOperator: public Ast_Expression {
  using Ast_Expression::Ast_Expression;

  ast_ptr<Ast_Expression> condition;
  ast_ptr<Ast_Expression> then_e;
  ast_ptr<Ast_Expression> else_e;
};

template<class Ret, class T1>
static auto visit_ast_decl(Ast_Expression &e,
                           Ret (&proc1)(T1)
                           ) -> Ret {
  auto ast_type = e.ast_type;
  if(ast_type == Ast_Variable_Type) {
    return proc1(static_cast<Ast_Variable &>(e));
  } else {
    assert(0);
    if constexpr(std::is_same<Ret, void>::value) {
      return;
    } else {
      return Ret{};
    }
  }
}

template<class Ret, class T1, class T2, class T3, class T4, class T5, class T6, class T7>
static auto visit_ast_expr(Ast_Expression &e,
                           Ret (&proc1)(T1),
                           Ret (&proc2)(T2),
                           Ret (&proc3)(T3),
                           Ret (&proc4)(T4),
                           Ret (&proc5)(T5),
                           Ret (&proc6)(T6),
                           Ret (&proc7)(T7)
                           ) -> Ret {
  auto ast_type = e.ast_type;
  if(ast_type == Ast_Literal_Type) {
    return proc1(static_cast<Ast_Literal &>(e));

  } else if(ast_type == Ast_AddOperator_Type) {
    return proc2(static_cast<Ast_AddOperator &>(e));

  } else if(ast_type == Ast_SubOperator_Type) {
    return proc3(static_cast<Ast_SubOperator &>(e));

  } else if(ast_type == Ast_MulOperator_Type) {
    return proc4(static_cast<Ast_MulOperator &>(e));

  } else if(ast_type == Ast_DivOperator_Type) {
    return proc5(static_cast<Ast_DivOperator &>(e));

  } else if(ast_type == Ast_UnaryMinus_Type) {
    return proc6(static_cast<Ast_UnaryMinus &>(e));

  } else if(ast_type == Ast_TernaryOperator_Type) {
    return proc7(static_cast<Ast_TernaryOperator &>(e));

  } else {
    assert(0);
    if constexpr(std::is_same<Ret, void>::value) {
      return;
    } else {
      return Ret{};
    }
  }
}


static auto destruct(Ast_Expression *expr) -> void {
  switch(expr->ast_type) {
    case Ast_Literal_Type: {
      delete static_cast<Ast_Literal*>(expr);
    } break;

    case Ast_Variable_Type: {
      delete static_cast<Ast_Variable *>(expr);
    } break;

    case Ast_AddOperator_Type: {
      delete static_cast<Ast_AddOperator *>(expr);
    } break;

    case Ast_SubOperator_Type: {
      delete static_cast<Ast_SubOperator *>(expr);
    } break;

    case Ast_MulOperator_Type: {
      delete static_cast<Ast_MulOperator *>(expr);
    } break;

    case Ast_DivOperator_Type: {
      delete static_cast<Ast_DivOperator *>(expr);
    } break;

    case Ast_UnaryMinus_Type: {
      delete static_cast<Ast_UnaryMinus *>(expr);
    } break;

    case Ast_TernaryOperator_Type: {
      delete static_cast<Ast_TernaryOperator *>(expr);
    } break;
  }
}

static auto parse_expr(u8) -> ast_ptr<Ast_Expression>;
static auto parse_unop(TokenType) -> ast_ptr<Ast_Expression>;

static auto binding_power(TokenType type) -> u8 {
  if(type == '+' || type == '-') {
    return 16;

  } else if(type == '*' || type == '/') {
    return 17;

  } else if(type == '?' || type == ':' ) {
    // ternary operator.
    return 7;

  } else if(type == '(' || type == ')' || type == ';') {
    return 0;

  } else { // @Why? ` ` also goes here.
    return 0;
  }
}


static auto parse_prefix_expr(Token tok) -> ast_ptr<Ast_Expression> {
  // @Note: Here we don't check tokens after prefixed expression.
  
  auto type = tok.type;
  if(type == LiteralTok) {
    // literal.
    auto expr = make_ast<Ast_Literal>(Ast_Literal_Type);
    expr->value = Value{tok.integer_value};
    return expr;

  } else if(type == IdentifierTok) {
    // variable or function call.
    return {}; // @Incomplete.

  } else if(is_unary_operator(type)) {
    // unary operator.
    return parse_unop(type);

  } else if(type == '(') {
    auto expr = parse_expr(binding_power(type));
    assert(next_token().type == ')');
    return expr;

  } else {
    // unexpected tok.
    return {};
  }

  assert(0);
  return {};
}

static auto parse_unary_minus() -> ast_ptr<Ast_Expression> {
  auto expr = make_ast<Ast_UnaryMinus>(Ast_UnaryMinus_Type);
  expr->operand = parse_prefix_expr(next_token());
  return expr;
}

static auto parse_unop(TokenType t) -> ast_ptr<Ast_Expression> {
  if(t == '+') {
    return parse_prefix_expr(next_token());

  } else if(t == '-') {
    return parse_unary_minus();

  } else {
    assert(0);
    return {};
  }
}

static auto parse_binop(ast_ptr<Ast_Expression> &&left, TokenType t) -> ast_ptr<Ast_Expression> {
  if(t == '+') {
    auto binop = make_ast<Ast_AddOperator>(Ast_AddOperator_Type);
    binop->left = std::move(left);
    binop->right = parse_expr(binding_power(t));
    return binop;

  } else if(t == '-') {
    auto binop = make_ast<Ast_SubOperator>(Ast_SubOperator_Type);
    binop->left = std::move(left);
    binop->right = parse_expr(binding_power(t));
    return binop;

  } else if(t == '*') {
    auto binop = make_ast<Ast_MulOperator>(Ast_MulOperator_Type);
    binop->left = std::move(left);
    binop->right = parse_expr(binding_power(t));
    return binop;

  } else if(t == '/') {
    auto binop = make_ast<Ast_DivOperator>(Ast_DivOperator_Type);
    binop->left = std::move(left);
    binop->right = parse_expr(binding_power(t));
    return binop;

  } else if(t == '?') {
    auto ternary_op = make_ast<Ast_TernaryOperator>(Ast_TernaryOperator_Type);
    ternary_op->condition = std::move(left);
    ternary_op->then_e = parse_expr(binding_power(t));

    auto type = next_token().type;
    assert(type == ':');

    ternary_op->else_e = parse_expr(binding_power(type)-1);
    return ternary_op;

  } else {
    assert(0);
    return {};
  }
}

static auto parse_expr(u8 current_bp) -> ast_ptr<Ast_Expression> {
  // Expression (can be evaluated into value).
  // It's:
  // 1) literal (1, 1.123, 'c', etc).
  // 2) existing variable.
  // 3) function call.
  // 4) ternary operator (`?:`)
  // 5) `expr  = unary_op `expr`.
  // 6) `expr` = ( `expr` ).
  // 7) `expr` = `expr` binary_op `expr`.

  auto left = parse_prefix_expr(next_token());

  while(binding_power(peek_token().type) > current_bp) {
    Token tok = next_token();
    auto type = tok.type;

    if(is_binary_operator(type)) { // left is part of a binary operator.
      left = parse_binop(std::move(left), type);

    } else if(type == ';') {
      return left;

    } else {
      // unexpected token.
      printf("%c\n", type);
      assert(0);
      return {};
    }
  }
  return left;
}


static auto parse_decl() -> ast_ptr<Ast_Expression> {
  // Top level declaration.
  // It's function decl, variable decl, or type decl.
  // Starts with `identifier-type` or `keyword`(ex. struct, union, typedef).

  Token tok = next_token();
  if(tok.type == EndTok) {
    return {};
  }

  if(tok.type == IdentifierTok) {
    // Function declaration or global variable.
    // @Note: for now it's only a variable.

    auto expr = make_ast<Ast_Variable>(Ast_Variable_Type);
    expr->type = tok.identifier_value;

    tok = next_token();
    if(tok.type != IdentifierTok) {
      // @Incomplete:
      // Report errors.
      return {};
    }

    expr->name = tok.identifier_value;
    expr->expr = parse_expr(0);
    return expr;
  }
  return {};
}

static auto interpret_expr(Ast_Expression &) -> Value;
static auto interpret_literal(const Ast_Literal &expr) -> Value {
  return expr.value;
}

static auto interpret_addop(const Ast_AddOperator &expr) -> Value {
  i32 v1 = interpret_expr(*expr.left).integer_value;
  i32 v2 = interpret_expr(*expr.right).integer_value;
  return Value {v1 + v2};
}

static auto interpret_subop(const Ast_SubOperator &expr) -> Value {
  i32 v1 = interpret_expr(*expr.left).integer_value;
  i32 v2 = interpret_expr(*expr.right).integer_value;
  return Value{v1 - v2};
}

static auto interpret_mulop(const Ast_MulOperator &expr) -> Value {
  i32 v1 = interpret_expr(*expr.left).integer_value;
  i32 v2 = interpret_expr(*expr.right).integer_value;
  return Value{v1 * v2};
}

static auto interpret_divop(const Ast_DivOperator &expr) -> Value {
  i32 v1 = interpret_expr(*expr.left).integer_value;
  i32 v2 = interpret_expr(*expr.right).integer_value;
  return Value{v1 / v2};
}

static auto interpret_unary_minus(const Ast_UnaryMinus &expr) -> Value {
  i32 v = interpret_expr(*expr.operand).integer_value;
  return Value{ -v };
}

static auto interpret_ternaryop(const Ast_TernaryOperator &t) -> Value {
  i32 condition = interpret_expr(*t.condition).integer_value;
  i32 then = interpret_expr(*t.then_e).integer_value;
  i32 else_e = interpret_expr(*t.else_e).integer_value;
  return Value{(condition) ? then : else_e };
                
                
}
                        

static auto interpret_expr(Ast_Expression &expr) -> Value {
  return visit_ast_expr(expr, 
                        interpret_literal, 
                        interpret_addop, 
                        interpret_subop, 
                        interpret_mulop, 
                        interpret_divop, 
                        interpret_unary_minus,
                        interpret_ternaryop);
}

static auto interpret_var(const Ast_Variable &decl) -> void {
  printf("Got: type="); print_string(decl.type);
  printf(", ident=");   print_string(decl.name);
  printf(", value=%i\n", interpret_expr(*decl.expr).integer_value);
}

static auto interpret_decl(Ast_Expression &expr) -> void {
  visit_ast_decl(expr,
                 interpret_var);
}


static auto parse_and_interp() -> void {
  ast_ptr<Ast_Expression> expr = parse_decl();
  while(expr) {
    assert(expr);
    interpret_decl(*expr);

    expr.reset();
    expr = parse_decl();
  }
}

static auto eval(const char *input) -> void {
  set_cursor(input);
  parse_and_interp();
}


#if 0
// Note: Needs to be global for Ctrl-C handler.
std::string line;
#endif

auto main() -> int {
  eval("int abc = 10 + 20");

#if 0
  // @Buggy: Handles Ctrl-C interruption
  struct sigaction act;
  act.sa_handler = [](int sig){
                      (void)sig;
                      std::cout << "\n";
                      line.clear();
                   };
  sigaction(SIGINT, &act, NULL);

  while(std::getline(std::cin, line)) {
    if(line.empty()) continue;

    eval(line.c_str());
    line.clear();
  }
#endif
}
