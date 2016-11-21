#include <memory>
#include <gtest/gtest.h>
#include <parser.h>

using namespace lexers;
using namespace parser;

TEST(ParserTest, NumberTest) {
    lexers::Lexer lex{"5"};
    auto exprPtr = parser::parseExpr(lex);
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(exprPtr));
    auto numPtr = std::dynamic_pointer_cast<NumberAST>(exprPtr);
    ASSERT_EQ(5, numPtr->getValue());
}

TEST(ParserTest, IdentifierTest) {
    lexers::Lexer lex{"abs"};
    auto exprPtr = parser::parseExpr(lex);
    ASSERT_TRUE(std::dynamic_pointer_cast<IdentifierAST>(exprPtr));
    auto idPtr = std::dynamic_pointer_cast<IdentifierAST>(exprPtr);
    ASSERT_STREQ("abs", idPtr->getId().c_str());

}
