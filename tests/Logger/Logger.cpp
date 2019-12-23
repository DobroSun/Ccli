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


inline std::string nl(const std::string &str) {
    return str + "\n";
}
TEST(LoggerCase, debug_works) {
    sstr res1 = init_read_buf();
    std::string expected1 = "PASS";
    debug() << expected1 << std::endl;
    ASSERT_EQ(res1.str(), nl(expected1));

    sstr res2 = init_read_buf();
    const char *expected2 = "PASS";
    debug() << expected2;
    ASSERT_EQ(res2.str(), expected2);

    sstr res3 = init_read_buf();
    int a = 3; 
    debug() << a << "Yeah";
    ASSERT_EQ(res3.str(), "3Yeah");

    sstr res4 = init_read_buf();
    std::string b = "foo()";
    debug() << a << 23 << b << std::endl;
    ASSERT_EQ(res4.str(), nl("323foo()"));


}
