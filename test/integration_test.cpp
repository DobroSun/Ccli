#include <gtest/gtest.h>
#include <string>

#include "ccli/exec_expr.hpp"


TEST(EvaluatingCase, Is_2_and_2_eq_4) {
    std::string res = exec_expr("2 + 3");
    ASSERT_EQ(res, "5");

    res = exec_expr("32432 * 4324");
    ASSERT_EQ(res, "140235968");

    res = exec_expr("324234 - 4532532432");
    ASSERT_EQ(res, "-4532208198");
}

TEST(EvaluatingCase, Bolean_tests) {
    std::string res = exec_expr("true");
    ASSERT_EQ(res, "1");

    res = exec_expr("false");
    ASSERT_EQ(res, "0");

    res = exec_expr("true && true");
    ASSERT_EQ(res, "1");

    res = exec_expr("true && false");
    ASSERT_EQ(res, "0");

    res = exec_expr("false && true");
    ASSERT_EQ(res, "0");

    res = exec_expr("false && false");
    ASSERT_EQ(res, "0");

    res = exec_expr("true || false");
    ASSERT_EQ(res, "1");

    res = exec_expr("false || true");
    ASSERT_EQ(res, "1");
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
