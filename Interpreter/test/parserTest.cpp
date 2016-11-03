#include <gtest/gtest.h>
#include <parser.h>
#include <lexers.h>

using namespace lexers;
using namespace parser;

TEST(ParserTest, NumberTest) {
    lexers::Lexer lex{"5"};
    auto exprPtr = parser::parseExpr(lex);
    ASSERT_TRUE(dynamic_cast<parser::NumberAST *>(exprPtr.get()));
    auto numPtr = dynamic_cast<parser::NumberAST *>(exprPtr.get());
    ASSERT_EQ(5, numPtr->getValue());
}

TEST(ParserTest, IdentifierTest) {
    lexers::Lexer lex{"abs"};
    auto exprPtr = parser::parseExpr(lex);
    ASSERT_TRUE(dynamic_cast<parser::IdentifierAST*>(exprPtr.get()));
    auto idPtr = dynamic_cast<parser::IdentifierAST*>(exprPtr.get());
    ASSERT_STREQ("abs", idPtr->getId().c_str());

}
