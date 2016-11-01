#include <gtest/gtest.h>
#include <lexers.h>

using namespace lexers;

TEST(LexersTest, NumberTest) {
    lexers::Lexer lex{"5"};
    ASSERT_EQ(Lexer::TokNumber, lex.getTokType());
    ASSERT_EQ(5, lex.getNum());
    ASSERT_EQ(Lexer::TokEOF, lex.getNextTok());
}

TEST(LexersTest, IdentifierTest) {
    lexers::Lexer lex{"abs"};
    ASSERT_EQ(Lexer::TokIdentifier, lex.getTokType());
    ASSERT_STREQ("abs", lex.getIdentifier().c_str());
    ASSERT_EQ(Lexer::TokEOF, lex.getNextTok());
}
