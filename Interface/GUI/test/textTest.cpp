#include <gtest/gtest.h>
#include <string>
#include <formatString.h>
using namespace con;

TEST(FormatTextTest, NormalCharTest) {
    FormatString formatString;
    pushString(formatString, "Hello world!");
    ASSERT_STREQ("Hello world!\n", formatString.toString().c_str());
}

TEST(FormatTextTest, lineFeedTest) {
    FormatString formatString;
    pushString(formatString, "(");
    setLineFeed(formatString, 3);
    pushString(formatString, ")");
    ASSERT_STREQ(""
                         "(\n"
                         "  \n"
                         "  \n"
                         "  )\n", formatString.toString().c_str());
}

TEST(FormatTextTest, backSpaceTest) {
    FormatString formatString;
    pushString(formatString, "(");
    formatString.lineFeedProcess();
    pushString(formatString, "bcd)");

    formatString.backSpaceProcess();
    formatString.backSpaceProcess();
    formatString.lineFeedProcess();
    formatString.normalCharProcess(')');

    ASSERT_STREQ(""
                         "(\n"
                         "  bc\n"
                         "  )\n", formatString.toString().c_str());
}

TEST(FormatTextTest, multiBracketLineFeedTest) {
    FormatString formatString;
    pushString(formatString, "(((");
    setLineFeed(formatString, 3);
    pushString(formatString, ")");
    formatString.lineFeedProcess();
    pushString(formatString, ")");
    formatString.lineFeedProcess();
    pushString(formatString, ")");
    ASSERT_STREQ(""
                         "(((\n"
                         "    \n"
                         "    \n"
                         "    )\n"
                         "   )\n"
                         "  )\n", formatString.toString().c_str());
}

TEST(FormatTextTest, MultiBracketBackSpaceTest) {
    FormatString formatString;
    pushString(formatString, "(((");
    setLineFeed(formatString, 3);
    pushString(formatString, ")");
    formatString.lineFeedProcess();
    pushString(formatString, ")");
    formatString.lineFeedProcess();
    pushString(formatString, ")");
    setBackSpace(formatString, 4);
    pushString(formatString, ")");
    formatString.lineFeedProcess();
    pushString(formatString, ")");
    ASSERT_STREQ(""
                         "(((\n"
                         "    \n"
                         "    \n"
                         "    ))\n"
                         "  )\n", formatString.toString().c_str());
}
