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


std::string wrp(const std::string &str) {
    return "\'" + str + "\'";
}
std::string nl(const std::string &str) {
    return str + "\n";
}
TEST(BashExecuteCase, exec) {
    if(!exec("which ghci").empty()) {
        std::string cmd1 = wrp("2 + 3");
        std::string cmd2 = wrp("print 15.02");
        std::string cmd3 = wrp(":t max");

        std::string wrapper = "ghci -e ";

        std::string res1 = exec(wrapper + cmd1);
        std::string res2 = exec(wrapper + cmd2);
        std::string res3 = exec(wrapper + cmd3);


        std::string expected1 = nl("5");
        std::string expected2 = nl("15.02");
        std::string expected3 = nl("max :: Ord a => a -> a -> a");
        ASSERT_EQ(res1, expected1);
        ASSERT_EQ(res2, expected2);
        ASSERT_EQ(res3, expected3);
    }
}

typedef std::ostringstream sstr;
sstr init_buf() {
    sstr res;
    std::cout.rdbuf(res.rdbuf());
    return res;
}
TEST(PrintCase, print) {
    sstr res1 = init_buf();
    std::vector<std::string> cmd1 = {
        "Foo", "Boo", "Bar",
        };
    std::string expected1 = "[Foo, Boo, Bar]\n";
    print(cmd1);
    ASSERT_EQ(res1.str(), expected1);


    sstr res2 = init_buf();
    std::vector<double> cmd2 = {
        2.32, 23.1, 324,
        };
    std::string expected2 = "[2.32, 23.1, 324]\n";
    print(cmd2);
    ASSERT_EQ(res2.str(), expected2);

}
