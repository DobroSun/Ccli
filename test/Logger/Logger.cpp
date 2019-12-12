#include "gtest/gtest.h"

#include "ccli/Logger.hpp"

#include <string>
#include <utility>

typedef std::ostringstream sstr;

sstr init_read_buf() {
    sstr res;
    std::cout.rdbuf(res.rdbuf());
    return res;
}

TEST(LoggerCase, debug_works) {
    // All tests before define DEBUG.
    // Have to fail because is running without DEBUG macro
    sstr res1 = init_read_buf();
    std::string expected1 = "FAIL";

    debug << expected1 << std::endl;
    ASSERT_EQ(res1.str(), "");

    #define DEBUG 1
    sstr res2 = init_read_bug();
    std::string expected2 = "PASS";

    debug << expected2 << std::endl;
    ASSERT_EQ(res2.str(), expected2 + "\n");


    sstr res3 = init_read_bug();
    const char *expected2 = "PASS";

    debug << expected2;
    ASSERT_EQ(res3.str(), expected2);
}
