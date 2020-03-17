#include "gtest/gtest.h"
#include <vector>
#include <array>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <deque>

#include "ccli/utility.hpp"
using namespace ccli::utility;

template<class T>
void check_eq(const T &a, const T &b) {
  ASSERT_EQ(a.size(), b.size());
  auto it1 = a.begin(); auto it2 = b.begin();
  for(; it1 != a.end() && it2 != b.end(); it1++, it2++) {
    EXPECT_EQ(*it1, *it2);
  }
  return;
}

using namespace ccli;

TEST(IsIterableCase, test_IO) {
  static_assert(is_iterable_v<std::vector<int>>, "");
  static_assert(is_iterable_v<std::array<int,1>>, "");
  static_assert(is_iterable_v<std::map<int,double>>, "");
  static_assert(is_iterable_v<std::multimap<int,double>>, "");
  static_assert(is_iterable_v<std::unordered_map<int,double>>, "");
  static_assert(is_iterable_v<std::list<int>>, "");
  static_assert(is_iterable_v<std::deque<int>>, "");


  static_assert(is_iterable_v<std::set<int>>, "");
  static_assert(is_iterable_v<std::multiset<double>>, "");
  static_assert(is_iterable_v<std::unordered_set<double>>, "");

  static_assert(!is_iterable_v<int>, "");
  static_assert(!is_iterable_v<double>, "");
  static_assert(!is_iterable_v<char*>, "");
};

TEST(IsMappingCase, test_IO) {
  static_assert(is_mapping_v<std::map<int, double>>, "");
  static_assert(is_mapping_v<std::unordered_map<int, double>>, "");
  static_assert(is_mapping_v<std::multimap<std::string, int>>, "");
  static_assert(!is_mapping_v<std::set<int>>, "");
  static_assert(!is_mapping_v<std::unordered_set<int>>, "");
  static_assert(!is_mapping_v<std::array<int, 3>>, "");
  static_assert(!is_mapping_v<std::vector<std::string>>, "");
  static_assert(!is_mapping_v<std::list<int>>, "");
  static_assert(!is_mapping_v<std::deque<int>>, "");
};

auto f1 = [](int &a) { a += 10; };
void f2(int &a) { a+= 10; }

auto ff1 = [](int a) { int b = a + 10; return b; };
int ff2(int a) { int b = a + 10; return b; };

auto fff1 = [](const int &a) { int b = a + 10; return b; };
int fff2(const int &a) { int b = a + 10; return b; };

//auto fff1 = [](int &a) { int b = a + 10; return b; }; <- fails.
//int fff2(int &a) { int b = a + 10; return b; }; <- fails.
// Might be fixed by adding constness to argument
// inside map function.
TEST(MapVector, test_insert) {
  std::vector<int> v1 = {1, 2, 3, 4, 5};
  std::vector<int> v2 = {1, 2, 3, 4, 5};
  std::vector<int> v3 = {1, 2, 3, 4, 5};
  std::vector<int> v4 = {1, 2, 3, 4, 5};
  std::vector<int> v5 = {1, 2, 3, 4, 5};
  std::vector<int> v6 = {1, 2, 3, 4, 5};

  std::vector<int> expected = {11, 12, 13, 14, 15};

  map(v1, f1);
  map(std::move(v2), f2);
  map(v3, ff1);
  map(std::move(v4), ff2);
  map(v5, fff1);
  map(std::move(v6), fff2);

  check_eq(v1, expected);
  check_eq(v2, expected);
  check_eq(v3, expected);
  check_eq(v4, expected);

  check_eq(v5, expected);
  check_eq(v6, expected);
};

TEST(MapArray, test_insert) {
  std::array<int, 5> a1 = {1, 2, 3, 4, 5};
  std::array<int, 5> a2 = {1, 2, 3, 4, 5};
  std::array<int, 5> a3 = {1, 2, 3, 4, 5};
  std::array<int, 5> a4 = {1, 2, 3, 4, 5};
  std::array<int, 5> a5 = {1, 2, 3, 4, 5};
  std::array<int, 5> a6 = {1, 2, 3, 4, 5};

  std::array<int, 5> expected = {11, 12, 13, 14, 15};

  map(a1, f1);
  map(std::move(a2), f2);
  map(a3, ff1);
  map(std::move(a4), ff2);
  map(a5, fff1);
  map(std::move(a6), fff2);

  check_eq(a1, expected);
  check_eq(a2, expected);
  check_eq(a3, expected);
  check_eq(a4, expected);
  check_eq(a5, expected);
  check_eq(a6, expected);
};

// Set totally fails.
TEST(MapSet, test_insert) {
/*
  std::set<int> s1 = {1, 2, 3, 4, 5};
  std::set<int> s2 = {1, 2, 3, 4, 5};
  std::set<int> s3 = {1, 2, 3, 4, 5};
  std::set<int> s4 = {1, 2, 3, 4, 5};
  std::set<int> s5 = {1, 2, 3, 4, 5};
  std::set<int> s6 = {1, 2, 3, 4, 5};
*/
  std::set<int> expected = {11, 12, 13, 14, 15};

/*
  map(s1, fff2);
  map(std::move(s2), f2);
  map(s3, ff1);
  map(std::move(s4), ff2);
  map(s5, fff1);
  map(std::move(s6), fff2);
*/

/*
  check_eq(s1, expected);
  check_eq(s2, expected);
  check_eq(s3, expected);
  check_eq(s4, expected);
  check_eq(s5, expected);
  check_eq(s6, expected);
*/
};
