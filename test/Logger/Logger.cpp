#include "gtest/gtest.h"

#define DEBUG
#include "ccli/Logger.hpp"

#include <string>
#include <utility>

typedef std::ostringstream sstr;

sstr init_read_buf() {
    sstr res;
    std::cout.rdbuf(res.rdbuf());
    return res;
}

template<typename T>
inline std::string stoi(T val) {
    return std::to_string(val);
}

TEST(LoggerCase, debug_works) {
    sstr res1 = init_read_buf();
    std::string expected1 = "PASS";

    debug() << expected1 << std::endl;
    ASSERT_EQ(res1.str(), expected1 + "\n");


    sstr res2 = init_read_buf();
    const char *expected2 = "PASS";

    debug() << expected2;
    ASSERT_EQ(res2.str(), expected2);
/*

    sstr res3 = init_read_buf();
    const int expected3 = 2;
    int expected4 = 2;
    int &expected5 = expected4;
    auto expected6 = 2;

    debug() << expected3;
    ASSERT_EQ(res3.str(), stoi(expected3));
    ASSERT_EQ(res3.str(), stoi(expected4));
    ASSERT_EQ(res3.str(), stoi(expected5));
    ASSERT_EQ(res3.str(), stoi(expected6));
    ASSERT_EQ(res3.str(), 2);
*/
}
