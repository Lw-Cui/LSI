#include <gtest/gtest.h>
#include <string>
#include <formatString.h>

TEST(FormatTextTest, NormalCharTest) {
    text::FormatString formatString;
    pushString(formatString, "Hello world!");
    ASSERT_STREQ("Hello world!\n", formatString.toString().c_str());
}

TEST(FormatTextTest, lineFeedTest) {
    text::FormatString formatString;
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
    text::FormatString formatString;
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
    text::FormatString formatString;
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
    text::FormatString formatString;
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
