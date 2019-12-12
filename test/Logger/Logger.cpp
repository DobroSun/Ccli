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


TEST(LoggerCase, debug_works) {
    sstr res2 = init_read_buf();
    std::string expected2 = "PASS";

    debug << expected2 << std::endl;
    ASSERT_EQ(res2.str(), expected2 + "\n");


    sstr res3 = init_read_buf();
    const char *expected3 = "PASS";

    debug << expected3;
    ASSERT_EQ(res3.str(), expected2);
}
