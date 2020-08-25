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

template<class Ret, class T>
static auto id(const T &) -> Ret {
  if constexpr(is_same<Ret, void>::value) {
    return;
  } else {
    return Ret{};
  }
}


template<class Ret, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
static auto visit_ast(Ast_Expression &e,
                      T1 proc1,
                      T2 proc2,
                      T3 proc3,
                      T4 proc4,
                      T5 proc5,
                      T6 proc6,
                      T7 proc7,
                      T8 proc8
                      ) -> Ret {

  auto ast_type = e.ast_type;
  switch(ast_type) {
    case Ast_Literal_Type:
      return proc1(static_cast<Ast_Literal &>(e));

    case Ast_AddOperator_Type:
      return proc2(static_cast<Ast_AddOperator &>(e));

    case Ast_SubOperator_Type:
      return proc3(static_cast<Ast_SubOperator &>(e));

    case Ast_MulOperator_Type:
      return proc4(static_cast<Ast_MulOperator &>(e));

    case Ast_DivOperator_Type:
      return proc5(static_cast<Ast_DivOperator &>(e));

    case Ast_UnaryMinus_Type:
      return proc6(static_cast<Ast_UnaryMinus &>(e));

    case Ast_TernaryOperator_Type:
      return proc7(static_cast<Ast_TernaryOperator &>(e));

    case Ast_Variable_Type:
      return proc8(static_cast<Ast_Variable &>(e));
  }
  return id<Ret, u8>(0);
}

template<class T>
struct default_deleter {
  auto operator()(T &o) -> void {
    return std::default_delete<T>{}(&o);
  }
};

static auto destruct(Ast_Expression *expr) -> void {
  visit_ast<void>(*expr,
                  default_deleter<Ast_Literal>{},
                  default_deleter<Ast_AddOperator>{},
                  default_deleter<Ast_SubOperator>{},
                  default_deleter<Ast_MulOperator>{},
                  default_deleter<Ast_DivOperator>{},
                  default_deleter<Ast_UnaryMinus>{},
                  default_deleter<Ast_TernaryOperator>{},
                  default_deleter<Ast_Variable>{}
                  );
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
    expr->value = tok.val;
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
    assert(0);
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
    
    assert(next_token().type == ';');
    return expr;
  }
  return {};
}

static auto interpret_expr(Ast_Expression &) -> Value;
static auto interpret_literal(const Ast_Literal &expr) -> Value {
  return expr.value;
}

static auto interpret_addop(const Ast_AddOperator &expr) -> Value {
  Value v1 = interpret_expr(*expr.left);
  Value v2 = interpret_expr(*expr.right);
  return v1 + v2;

}

static auto interpret_subop(const Ast_SubOperator &expr) -> Value {
  Value v1 = interpret_expr(*expr.left);
  Value v2 = interpret_expr(*expr.right);
  return v1 - v2;
}

static auto interpret_mulop(const Ast_MulOperator &expr) -> Value {
  Value v1 = interpret_expr(*expr.left);
  Value v2 = interpret_expr(*expr.right);
  return v1 * v2;
}

static auto interpret_divop(const Ast_DivOperator &expr) -> Value {
  Value v1 = interpret_expr(*expr.left);
  Value v2 = interpret_expr(*expr.right);
  return v1 / v2;
}

static auto interpret_unary_minus(const Ast_UnaryMinus &expr) -> Value {
  Value v = interpret_expr(*expr.operand);
  return -v;
}

static auto interpret_ternaryop(const Ast_TernaryOperator &t) -> Value {
  Value condition = interpret_expr(*t.condition);
  Value then_e = interpret_expr(*t.then_e);
  Value else_e = interpret_expr(*t.else_e);
  return (to_bool(condition)) ? then_e: else_e;
}
                        
static auto interpret_var(const Ast_Variable &decl) -> void {
  printf("Got: type="); print_string(decl.type);
  printf(", ident=");   print_string(decl.name);
  printf("%s", ", value=");
  print_value(interpret_expr(*decl.expr));
  puts("");
}

static auto interpret_expr(Ast_Expression &expr) -> Value {
  return visit_ast<Value>(expr, 
                          interpret_literal, 
                          interpret_addop, 
                          interpret_subop, 
                          interpret_mulop, 
                          interpret_divop, 
                          interpret_unary_minus,
                          interpret_ternaryop,
                          id<Value, Ast_Variable>
                          );
}

static auto interpret_decl(Ast_Expression &expr) -> void {
  return visit_ast<void>(expr, 
                         id<void, Ast_Literal>,
                         id<void, Ast_AddOperator>,
                         id<void, Ast_SubOperator>,
                         id<void, Ast_MulOperator>,
                         id<void, Ast_DivOperator>,
                         id<void, Ast_UnaryMinus>,
                         id<void, Ast_TernaryOperator>,
                         interpret_var
                         );

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
  eval("/* Hello /* world*/ */ int abc = 10 + .2 / .2;");
  eval("// int foo = bar;\n char b = '1' + 1.50;"); // '1' is 49, so got: 50.5


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
