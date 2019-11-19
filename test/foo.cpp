#include <gtest/gtest.h>
#include "ccli/helpers.hpp"

TEST(sample_test_case, sample_test) {
    ASSERT_EQ(1, 1);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
