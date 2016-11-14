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

TEST(ParserTest, ConditionTest) {
    Scope ss;
    lexers::Lexer lex{"(define n 0)"};
    parseExpr(lex)->eval(ss);
    ASSERT_FALSE(ss["n"]->toBool());
}

TEST(ParserTest, AddOperatorTest) {
    Scope ss;
    lexers::Lexer lex{"(+ 5 6 7)"};
    auto exprPtr = parseExpr(lex)->eval(ss);
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(exprPtr));
    auto numPtr = std::dynamic_pointer_cast<NumberAST>(exprPtr);
    ASSERT_EQ(18, numPtr->getValue());
}

TEST(ParserTest, IdentifierDefinitionTest) {
    Scope ss;
    lexers::Lexer lex{"(define n 5)"};

    auto exprPtr = parser::parseExpr(lex)->eval(ss);
    ASSERT_TRUE(ss.count("n"));
    exprPtr = ss["n"]->eval(ss);

    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(exprPtr));
    auto numPtr = std::dynamic_pointer_cast<NumberAST>(exprPtr);
    ASSERT_EQ(5, numPtr->getValue());
    ASSERT_EQ(Lexer::TokEOF, lex.getTokType());

    lex.appendExp("(define a n)");
    exprPtr = parseExpr(lex)->eval(ss);

    ASSERT_TRUE(ss.count("a"));

    exprPtr = ss["a"]->eval(ss);
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(exprPtr));
    numPtr = std::dynamic_pointer_cast<NumberAST>(exprPtr);
    ASSERT_EQ(5, numPtr->getValue());
    ASSERT_EQ(Lexer::TokEOF, lex.getTokType());
}

TEST(ParserTest, FunctionDefinitionTest) {
    Scope ss;
    lexers::Lexer lex{"(define (foo x) x)"};
    auto exprPtr = parser::parseExpr(lex)->eval(ss);
    ASSERT_TRUE(ss.count("foo"));
}

TEST(ParserTest, FunctionApplicationTest) {
    Scope ss;
    lexers::Lexer lex{"(define (foo x) x)"};
    auto exprPtr = parser::parseExpr(lex)->eval(ss);
    ASSERT_TRUE(ss.count("foo"));

    lex.appendExp("(foo 5)");
    exprPtr = parseExpr(lex)->eval(ss);
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(exprPtr));
    auto numPtr = std::dynamic_pointer_cast<NumberAST>(exprPtr);
    ASSERT_EQ(5, numPtr->getValue());

    lex.appendExp("(define (bar x) (+ x 1))");
    exprPtr = parseExpr(lex)->eval(ss);
    ASSERT_TRUE(ss.count("bar"));

    lex.appendExp("(bar 4)");
    exprPtr = parseExpr(lex)->eval(ss);
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(exprPtr));
    numPtr = std::dynamic_pointer_cast<NumberAST>(exprPtr);
    ASSERT_EQ(5, numPtr->getValue());

    lex.appendExp("(define n 4)");
    exprPtr = parseExpr(lex)->eval(ss);
    ASSERT_TRUE(ss.count("n"));

    lex.appendExp("(bar n)");
    exprPtr = parseExpr(lex)->eval(ss);
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(exprPtr));
    numPtr = std::dynamic_pointer_cast<NumberAST>(exprPtr);
    ASSERT_EQ(5, numPtr->getValue());

    lex.appendExp("(define b (foo (bar n)))");
    exprPtr = parseExpr(lex)->eval(ss);
    ASSERT_TRUE(ss.count("b"));
    exprPtr = ss["b"];
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(exprPtr));
    numPtr = std::dynamic_pointer_cast<NumberAST>(exprPtr);
    ASSERT_EQ(5, numPtr->getValue());
}

TEST(ParserTest, FunctionWithMultipleArgumentsTest) {
    Scope ss;
    lexers::Lexer lex("(define (bar x y) (+ x y 1))");
    auto exprPtr = parseExpr(lex)->eval(ss);
    ASSERT_TRUE(ss.count("bar"));

    lex.appendExp("(define n 5)");
    parseExpr(lex)->eval(ss);

    lex.appendExp("(bar n 4)");
    exprPtr = parseExpr(lex)->eval(ss);
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(exprPtr));
    auto numPtr = std::dynamic_pointer_cast<NumberAST>(exprPtr);
    ASSERT_EQ(10, numPtr->getValue());
}

TEST(ParserTest, HighOrderFunctionTEST) {
    Scope ss;
    lexers::Lexer lex("(define (bar f x) (+ (f x) 1))");
    parseExpr(lex)->eval(ss);
    ASSERT_TRUE(ss.count("bar"));

    lex.appendExp("(define (foo x) (+ x 1))");
    parseExpr(lex)->eval(ss);
    ASSERT_TRUE(ss.count("foo"));

    lex.appendExp("(define n 4)");
    parseExpr(lex)->eval(ss);
    ASSERT_TRUE(ss.count("n"));

    lex.appendExp("(bar foo n)");
    auto exprPtr = parseExpr(lex)->eval(ss);
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(exprPtr));
    auto numPtr = std::dynamic_pointer_cast<NumberAST>(exprPtr);
    ASSERT_EQ(6, numPtr->getValue());
}


TEST(ParserTest, IfStatementTest) {
    Scope ss;
    lexers::Lexer lex("(if (+ 5 6) 5 6)");
    auto exprPtr = parseExpr(lex)->eval(ss);
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(exprPtr));
    auto numPtr = std::dynamic_pointer_cast<NumberAST>(exprPtr);
    ASSERT_EQ(5, numPtr->getValue());

    lex.appendExp("(define (add x y) (+ x y))");
    parseExpr(lex)->eval(ss);
    lex.appendExp("(define (bar x) (+ x 1))");
    parseExpr(lex)->eval(ss);
    lex.appendExp("(define (foo x) (+ x 2))");
    parseExpr(lex)->eval(ss);
    ASSERT_TRUE(ss.count("foo"));

    lex.appendExp("((if (add 0 0) bar foo) 0)");
    exprPtr = parseExpr(lex)->eval(ss);
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(exprPtr));
    numPtr = std::dynamic_pointer_cast<NumberAST>(exprPtr);
    ASSERT_EQ(2, numPtr->getValue());
}
