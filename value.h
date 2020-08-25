#ifndef CCLI_VALUE_H
#define CCLI_VALUE_H
#include "utility.h"
#include <cassert>

enum ValueTag {
  U8,  // char
  I32, // int
  F32, // float
};

struct Value {
  ValueTag tag;

  union {
    u8  char_value;
    i32 integer_value;
    f32 float_value;
  };
};

template<class Ret, class T1, class T2, class T3>
auto visit_value(Value v,
                 T1 proc1,
                 T2 proc2,
                 T3 proc3
                 ) -> Ret {
  switch(v.tag) {
    case I32: {
      return proc1(v.integer_value);
    } break;

    case F32: {
      return proc2(v.float_value);
    } break;

    case U8: {
      return proc3(v.char_value);
    } break;
  }
  assert(0);
  if constexpr(is_same<Ret, void>::value) {
    return;
  } else {
    return Ret{};
  }
}

struct add {
  template<class T1, class T2>
  auto operator()(T1 v1, T2 v2) -> decltype(v1 + v2) {
    return v1 + v2;
  }
};

struct sub {
  template<class T1, class T2>
  auto operator()(T1 v1, T2 v2) -> decltype(v1 - v2) {
    return v1 - v2;
  }
};

struct mul {
  template<class T1, class T2>
  auto operator()(T1 v1, T2 v2) -> decltype(v1 * v2) {
    return v1 * v2;
  }
};

struct div {
  template<class T1, class T2>
  auto operator()(T1 v1, T2 v2) -> decltype(v1 / v2) {
    return v1 / v2;
  }
};

struct negate {
  template<class T>
  auto operator()(T v) -> decltype(-v) {
    return -v;
  }
};


template<class T1, class T2, class Op>
auto value_from_binop(T1 t1, T2 t2, Op op) -> Value {
  Value ret;
  if constexpr(is_same<i32, decltype(op(t1, t2))>::value) {
    i32 r = op(t1, t2);
    ret.integer_value = r;
    ret.tag = I32;

  } else if(is_same<f32, decltype(op(t1, t2))>::value) {
    f32 r = op(t1, t2);
    ret.float_value = r;
    ret.tag = F32;

  } else if(is_same<u8, decltype(op(t1, t2))>::value) {
    u8 r = op(t1, t2);
    ret.char_value = r;
    ret.tag = U8;

  } else {
    assert(0);
  }
  return ret;
}

template<class T, class Op>
auto value_from_unop(T t, Op op) -> Value {
  Value ret;
  if constexpr(is_same<i32, decltype(op(t))>::value) {
    i32 r = op(t);
    ret.integer_value = r;
    ret.tag = I32;

  } else if(is_same<f32, decltype(op(t))>::value) {
    f32 r = op(t);
    ret.float_value = r;
    ret.tag = F32;

  } else if(is_same<u8, decltype(op(t))>::value) {
    u8 r = op(t);
    ret.char_value = r;
    ret.tag = U8;

  } else {
    assert(0);
  }
  return ret;
}

template<class T1, class T2, class Op>
auto partial_binop(T1 t1, Op op) {
  return [=](T2 t2) {
    return value_from_binop(t1, t2, op);
  };
}

template<class T, class Op>
auto partial_unop(Op op) {
  return [=](T t) {
    return value_from_unop(t, op);
  };
}

template<class T, class Op>
auto visit_and_do_binop(Value v, Op op) {
  return [=](T t) {
    return visit_value<Value>(v,
                              partial_binop<T, i32>(t, op),
                              partial_binop<T, f32>(t, op),
                              partial_binop<T, u8>(t, op)
                              );
                              
  };
}

template<class BinaryOperator>
auto base_bin_operator(Value v1, Value v2, BinaryOperator op) -> Value {
  return visit_value<Value>(v1,
                            visit_and_do_binop<i32>(v2, op),
                            visit_and_do_binop<f32>(v2, op),
                            visit_and_do_binop<u8>(v2, op)
                           );
}

inline auto operator+(Value v1, Value v2) -> Value {
  return base_bin_operator(v1, v2, add{});
}

inline auto operator-(Value v1, Value v2) -> Value {
  return base_bin_operator(v1, v2, sub{});
}

inline auto operator*(Value v1, Value v2) -> Value {
  return base_bin_operator(v1, v2, mul{});
}

inline auto operator/(Value v1, Value v2) -> Value {
  return base_bin_operator(v1, v2, div{});
}


template<class T>
auto to_bool(T i) -> bool {
  return i != 0;
}

template<>
inline auto to_bool<Value>(Value v) -> bool {
  return visit_value<bool>(v,
                           to_bool<i32>,
                           to_bool<f32>,
                           to_bool<u8>
                           );
}

inline auto operator-(Value v) -> Value {
  return visit_value<Value>(v,
                            partial_unop<i32>(negate{}),
                            partial_unop<f32>(negate{}),
                            partial_unop<u32>(negate{})
                            );
}


inline auto print_value(Value v) -> void {
  visit_value<void>(v,
                    [](i32 i) { printf("%i", i); },
                    [](f32 i) { printf("%f", i); },
                    [](u8 i) { printf("%c", i); }
                    );
}

#endif
