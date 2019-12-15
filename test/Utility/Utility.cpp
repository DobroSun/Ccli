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

TEST(SplitCase, split) {
    std::string cmd1 = "int a;\nbool b;\nvoid foo();";
    std::string cmd2 = "a\n";
    std::string cmd3 = "b";
    std::string cmd4 = "This:is:gonna:be:the:greatest:project:in:the:whole:world:!";


    std::vector<std::string> expected1 = {
        "int a;",
        "bool b;",
        "void foo();"};
    std::vector<std::string> expected2 = {
        "a"};
    std::vector<std::string> expected3 = {
        "b"};
    std::vector<std::string> expected4 = {
        "This", "is", "gonna", "be", "the",
        "greatest", "project", "in",
        "the", "whole", "world", "!"};


    std::vector<std::string> res1 = split(cmd1);
    std::vector<std::string> res2 = split(cmd2);
    std::vector<std::string> res3 = split(cmd3);
    std::vector<std::string> res4 = split(cmd4, ':');


    ASSERT_EQ(res1, expected1);
    ASSERT_EQ(res2, expected2);
    ASSERT_EQ(res3, expected3);
    ASSERT_EQ(res4, expected4);
}


TEST(BashExecuteCase, exec) {
    if(!exec("which ghci").empty()) {
        std::string cmd1 = "\'2 + 3\'";
        std::string cmd2 = "\'print 15.02\'";
        std::string cmd3 = "\':t max\'";

        std::string wrapper = "ghci -e ";

        std::string res1 = exec(wrapper + cmd1);
        std::string res2 = exec(wrapper + cmd2);
        std::string res3 = exec(wrapper + cmd3);


        std::string expected1 = "5\n";
        std::string expected2 = "15.02\n";
        std::string expected3 = "max :: Ord a => a -> a -> a\n";
        ASSERT_EQ(res1, expected1);
        ASSERT_EQ(res2, expected2);
        ASSERT_EQ(res3, expected3);
    }
}
