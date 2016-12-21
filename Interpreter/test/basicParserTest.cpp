#include <memory>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <parser.h>

using namespace lexers;
using namespace parser;

class MockLexer : public lexers::Lexer {
public:
    MockLexer() : Lexer("") {}

    MOCK_METHOD0(getNum, double(void));

    MOCK_METHOD0(getIdentifier, std::string(void));

    MOCK_CONST_METHOD0(getTokType, Lexer::TokenType(void));

};

TEST(ParserTest, NumberTest) {
    MockLexer lex;
    EXPECT_CALL(lex, getNum()).WillOnce(testing::Return(5));
    EXPECT_CALL(lex, getTokType()).WillRepeatedly(testing::Return(lexers::Lexer::TokNumber));

    auto exprPtr = parser::parseExpr(lex);
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(exprPtr));
    auto numPtr = std::dynamic_pointer_cast<NumberAST>(exprPtr);
    ASSERT_EQ(5, numPtr->getValue());
}

TEST(ParserTest, IdentifierTest) {
    MockLexer lex;
    EXPECT_CALL(lex, getIdentifier()).WillOnce(testing::Return("abs"));
    EXPECT_CALL(lex, getTokType()).WillRepeatedly(testing::Return(lexers::Lexer::TokIdentifier));

    auto exprPtr = parser::parseExpr(lex);
    ASSERT_TRUE(std::dynamic_pointer_cast<IdentifierAST>(exprPtr));
    auto idPtr = std::dynamic_pointer_cast<IdentifierAST>(exprPtr);
    ASSERT_STREQ("abs", idPtr->getId().c_str());

}
