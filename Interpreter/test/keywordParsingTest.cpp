#include <memory>
#include <gtest/gtest.h>
#include <parser.h>

using namespace lexers;
using namespace parser;

TEST(KeywordParsingTest, IdentifierDefinitionTest) {
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


TEST(KeywordParsingTest, IfStatementTest) {
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

TEST(KeywordParsingTest, BooleansTest) {
    Scope ss;
    lexers::Lexer lex("#t");
    ASSERT_TRUE(parseExpr(lex)->eval(ss));
    lex.appendExp("#f");
    ASSERT_FALSE(parseExpr(lex)->eval(ss));
}

TEST(KeywordParsingTest, LoadingFileTest) {
    Scope s;
    lexers::Lexer lex("(load \"Test.scm\")");
    parseExpr(lex)->eval(s);
    ASSERT_TRUE(s.count("foo"));
    ASSERT_TRUE(s.count("add"));

    lex.appendExp("(foo 5)");
    auto res = parseExpr(lex)->eval(s);
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(res));
    auto numPtr = std::dynamic_pointer_cast<NumberAST>(res);
    ASSERT_EQ(6, numPtr->getValue());

    lex.appendExp("(define n 5)").appendExp("(add n 6)");
    res = parseAllExpr(lex)->eval(s);

    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(res));
    numPtr = std::dynamic_pointer_cast<NumberAST>(res);
    ASSERT_EQ(11, numPtr->getValue());
}

TEST(KeywordParsingTest, NilTest) {
    Scope s;
    lexers::Lexer lex;
    lex.appendExp("(define p (cons 1 nil))").appendExp("(cdr p)");

    auto res = parseAllExpr(lex)->eval(s);
    ASSERT_TRUE(std::dynamic_pointer_cast<NilAST>(res));
}
