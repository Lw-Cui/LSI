#include <gtest/gtest.h>
#include <lexers.h>

using namespace lexers;

TEST(LexersTest, BaseTest) {
    lexers::Lexer lex{"5"};
    ASSERT_EQ(Lexer::TokNumber, lex.getTokType());
    ASSERT_EQ(Lexer::TokEOF, lex.getNextTok());
}