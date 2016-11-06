#include <memory>
#include <gtest/gtest.h>
#include <parser.h>

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
    ASSERT_TRUE(dynamic_cast<parser::IdentifierAST *>(exprPtr.get()));
    auto idPtr = dynamic_cast<parser::IdentifierAST *>(exprPtr.get());
    ASSERT_STREQ("abs", idPtr->getId().c_str());

}

TEST(ParserTest, IdentifierDefinitionTest) {
    lexers::Lexer lex{"(define n 5)"};
    std::shared_ptr<ExprAST> exprPtr;
    std::shared_ptr<IdentifierAST> idPtr;
    Scope ss;

    exprPtr = parser::parseExpr(lex);
    exprPtr->eval(ss);
    ASSERT_TRUE(ss.count("n"));
    exprPtr = ss["n"]->eval(ss);

    ASSERT_TRUE(dynamic_cast<parser::NumberAST *>(exprPtr.get()));
    auto numPtr = dynamic_cast<parser::NumberAST *>(exprPtr.get());
    ASSERT_EQ(5, numPtr->getValue());
    ASSERT_EQ(Lexer::TokEOF, lex.getTokType());

    lexers::Lexer lex2{"(define a n)"};
    auto ptr = parser::parseExpr(lex2);
    ptr->eval(ss);

    ASSERT_TRUE(ss.count("a"));
    ptr = ss["a"]->eval(ss);

    ASSERT_TRUE(dynamic_cast<parser::NumberAST *>(exprPtr.get()));
    numPtr = dynamic_cast<parser::NumberAST *>(exprPtr.get());
    ASSERT_EQ(5, numPtr->getValue());
    ASSERT_EQ(Lexer::TokEOF, lex.getTokType());
}
