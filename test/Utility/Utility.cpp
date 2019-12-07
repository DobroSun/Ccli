#include "gtest/gtest.h"

#include "ccli/Utility.hpp"


TEST(JoinCase, join) {
    std::vector<std::string> cmds1 = {
        "a",
        "b",
        "c",
        };
    std::vector<std::string> cmds2 = {
        "int main() {",
        "return 0;",
        "}",
        };
    std::vector<std::string> cmds3;

    std::string expected = "a\nb\nc\n";
    std::string expected2 = "int main() {\nreturn 0;\n}\n";
    std::string expected3 = "";

    std::string res1 = join(cmds1);
    std::string res2 = join(cmds2);
    std::string res3 = join(cmds3);

    ASSERT_EQ(res1, expected);
    ASSERT_EQ(res2, expected2);
    ASSERT_EQ(res3, expected3);
}
