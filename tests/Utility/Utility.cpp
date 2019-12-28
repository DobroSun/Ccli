#include "gtest/gtest.h"

#include "ccli/Utility.hpp"

// Is needed for print_function.
#define DEBUG


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


TEST(TrimCase, ltrim) {
    std::string cmd1 = "    foo";
    std::string cmd2 = "  f  ";
    std::string cmd3 = "foo   ";
    std::string cmd4 = "vo    id";

    std::string res1 = ltrim(cmd1);
    std::string res2 = ltrim(cmd2);
    std::string res3 = ltrim(cmd3);
    std::string res4 = ltrim(cmd4);

    std::string expected1 = "foo";
    std::string expected2 = "f  ";
    std::string expected3 = cmd3;
    std::string expected4 = cmd4;


    ASSERT_EQ(res1, expected1);
    ASSERT_EQ(res2, expected2);
    ASSERT_EQ(res3, expected3);
    ASSERT_EQ(res4, expected4);
}

TEST(TrimCase, rtrim) {
    std::string cmd1 = "    foo";
    std::string cmd2 = "  f  ";
    std::string cmd3 = "foo   ";
    std::string cmd4 = "vo    id";

    std::string res1 = rtrim(cmd1);
    std::string res2 = rtrim(cmd2);
    std::string res3 = rtrim(cmd3);
    std::string res4 = rtrim(cmd4);

    std::string expected1 = cmd1;
    std::string expected2 = "  f";
    std::string expected3 = "foo";
    std::string expected4 = cmd4;


    ASSERT_EQ(res1, expected1);
    ASSERT_EQ(res2, expected2);
    ASSERT_EQ(res3, expected3);
    ASSERT_EQ(res4, expected4);
}

TEST(TrimCase, trim) {
    std::string cmd1 = "    foo";
    std::string cmd2 = "  f  ";
    std::string cmd3 = "foo   ";
    std::string cmd4 = "vo    id";
    std::string cmd5 = " foo  bar   baz  ";

    std::string res1 = trim(cmd1);
    std::string res2 = trim(cmd2);
    std::string res3 = trim(cmd3);
    std::string res4 = trim(cmd4);
    std::string res5 = trim(cmd5);

    std::string expected1 = "foo";
    std::string expected2 = "f";
    std::string expected3 = "foo";
    std::string expected4 = "vo id";
    std::string expected5 = "foo bar baz";


    ASSERT_EQ(res1, expected1);
    ASSERT_EQ(res2, expected2);
    ASSERT_EQ(res3, expected3);
    ASSERT_EQ(res4, expected4);
    ASSERT_EQ(res5, expected5);
}

double dob(double &n) {
    n *= n;
    return n;
}
std::string oh(std::string n) {
    return n + "!";
}
TEST(MapCase, map) {
    std::vector<int> cmd1 = {1, 2, 3, 4};
    std::vector<double> cmd2 = {1, 2, 3, 4};
    std::vector<std::string> cmd3 = {"l", "foo", "bar"};

    std::vector<int> res1 = map([](int &n){ return ++n;}, cmd1);
    std::vector<double> res2 = map(dob, cmd2);
    std::vector<std::string> res3 = map(oh, cmd3);
    std::vector<std::string> res4 = map(oh, std::vector<std::string>({"l", "foo", "bar"}));

    std::vector<int> expected1 = {2, 3, 4, 5};
    std::vector<double> expected2 = {1, 4, 9, 16};
    std::vector<std::string> expected3 = {"l!", "foo!", "bar!"};
    std::vector<std::string> expected4 = {"l!", "foo!", "bar!"};


    ASSERT_EQ(res1, expected1);
    ASSERT_EQ(res2, expected2);
    ASSERT_EQ(res3, expected3);
    ASSERT_EQ(res4, expected4);
}
