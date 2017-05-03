#include <memory>
#include <gtest/gtest.h>
#include <exception.h>
#include <parser.h>

using namespace lexers;
using namespace parser;
using namespace exception;

TEST(KeywordParsingTest, IdentifierDefinitionTest) {
    auto ss = std::make_shared<Scope>();
    lexers::Lexer lex{"(define n 5)"};

    auto ast = parser::parseExpr(lex);
    auto exprPtr = ast->eval(ss, ast);
    ASSERT_TRUE(ss->count("n"));
    exprPtr = ss->searchName("n")->eval(ss, ss->searchName("n"));

    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(exprPtr));
    auto numPtr = std::dynamic_pointer_cast<NumberAST>(exprPtr);
    ASSERT_EQ(5, numPtr->getValue());
    ASSERT_EQ(Lexer::TokEOF, lex.getTokType());

    lex.appendExp("(define a n)");
    ast = parser::parseExpr(lex);
    exprPtr = ast->eval(ss, ast);
    ASSERT_TRUE(ss->count("a"));

    exprPtr = ss->searchName("a")->eval(ss, ss->searchName("a"));
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(exprPtr));
    numPtr = std::dynamic_pointer_cast<NumberAST>(exprPtr);
    ASSERT_EQ(5, numPtr->getValue());
    ASSERT_EQ(Lexer::TokEOF, lex.getTokType());
}

TEST(KeywordParsingTest, IfStatementTest) {
    auto ss = std::make_shared<Scope>();
    lexers::Lexer lex;
    lex.appendExp("(if (+ 5 6) 5 6)");
    auto ast = parser::parseExpr(lex);
    auto exprPtr = ast->eval(ss, ast);
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(exprPtr));
    auto numPtr = std::dynamic_pointer_cast<NumberAST>(exprPtr);
    ASSERT_EQ(5, numPtr->getValue());

    lex.appendExp("(load \"setup.scm\")").appendExp("(define (add x y) (+ x y))");
    ast = parseAllExpr(lex);
    ast->eval(ss, ast);
    lex.appendExp("(define (bar x) (+ x 1))");
    ast = parseExpr(lex);
    ast->eval(ss, ast);
    lex.appendExp("(define (foo x) (+ x 2))");
    ast = parseExpr(lex);
    ast->eval(ss, ast);
    ASSERT_TRUE(ss->count("foo"));

    lex.appendExp("((if (= (add 0 0) 1) bar foo) 0)");
    ast = parseExpr(lex);
    exprPtr = ast->eval(ss, ast);
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(exprPtr));
    numPtr = std::dynamic_pointer_cast<NumberAST>(exprPtr);
    ASSERT_EQ(2, numPtr->getValue());
}

TEST(KeywordParsingTest, CondStatementTest) {
    auto ss = std::make_shared<Scope>();
    lexers::Lexer lex;
    lex.appendExp("(load \"setup.scm\")")
            .appendExp("(cond ((= (+ 5 6) 0) 1)"
                               "       ((= (+ 5 (- 4)) 0) 2)"
                               "       (else 3))");
    auto ast = parser::parseAllExpr(lex);
    auto exprPtr = ast->eval(ss, ast);
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(exprPtr));
    auto numPtr = std::dynamic_pointer_cast<NumberAST>(exprPtr);
    ASSERT_EQ(3, numPtr->getValue());

    lex.appendExp("(cond ((= (+ 5 6) 0) 1)"
                          "       ((= (+ 5 (- 5)) 0) 2)"
                          "       (else 3))");
    ast = parseAllExpr(lex);
    exprPtr = ast->eval(ss, ast);
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(exprPtr));
    numPtr = std::dynamic_pointer_cast<NumberAST>(exprPtr);
    ASSERT_EQ(2, numPtr->getValue());
}


TEST(KeywordParsingTest, LetStatementTest) {
    auto ss = std::make_shared<Scope>();
    lexers::Lexer lex;
    lex.appendExp("(load \"setup.scm\")")
            .appendExp("(let((x 5)"
                               "      (y 6)"
                               "      (foo (lambda (x y) (+ x y))))"
                               "  (foo x y))");
    auto ast = parser::parseAllExpr(lex);
    auto exprPtr = ast->eval(ss, ast);
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(exprPtr));
    auto numPtr = std::dynamic_pointer_cast<NumberAST>(exprPtr);
    ASSERT_EQ(11, numPtr->getValue());
}

TEST(KeywordParsingTest, BooleansTest) {
    auto ss = std::make_shared<Scope>();
    lexers::Lexer lex("#t");
    auto ast = parseExpr(lex);
    ASSERT_TRUE(std::dynamic_pointer_cast<BooleansTrueAST>(ast->eval(ss, ast)));
    lex.appendExp("#f");
    ast = parseExpr(lex);
    ASSERT_TRUE(std::dynamic_pointer_cast<BooleansFalseAST>(ast->eval(ss, ast)));
}

TEST(KeywordParsingTest, LoadingFileTest) {
    auto s = std::make_shared<Scope>();
    lexers::Lexer lex("(load \"Test.scm\")");
    auto ast = parseExpr(lex);
    ast->eval(s, ast);
    ASSERT_TRUE(s->count("foo"));
    ASSERT_TRUE(s->count("add"));

    lex.appendExp("(foo 5)");
    ast = parseExpr(lex);
    auto res = ast->eval(s, ast);
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(res));
    auto numPtr = std::dynamic_pointer_cast<NumberAST>(res);
    ASSERT_EQ(6, numPtr->getValue());

    lex.appendExp("(define n 5)").appendExp("(add n 6)");
    ast = parseAllExpr(lex);
    res = ast->eval(s, ast);

    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(res));
    numPtr = std::dynamic_pointer_cast<NumberAST>(res);
    ASSERT_EQ(11, numPtr->getValue());
}

TEST(KeywordParsingTest, NilTest) {
    auto s = std::make_shared<Scope>();
    lexers::Lexer lex;
    lex.appendExp("(define p (cons 1 nil))").appendExp("(cdr p)");

    auto ast = parseAllExpr(lex);
    auto res = ast->eval(s, ast);
    ASSERT_TRUE(std::dynamic_pointer_cast<NilAST>(res));
    auto nil = std::dynamic_pointer_cast<NilAST>(res);
    ASSERT_STREQ("\'()", nil->display().c_str());
}

