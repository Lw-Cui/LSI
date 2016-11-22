#include <memory>
#include <gtest/gtest.h>
#include <parser.h>

using namespace lexers;
using namespace parser;

TEST(FunctionParsingTest, FunctionDefinitionTest) {
    Scope ss;
    lexers::Lexer lex{"(define (foo x) x)"};
    auto exprPtr = parser::parseExpr(lex)->eval(ss);
    ASSERT_TRUE(ss.count("foo"));
}

TEST(FunctionParsingTest, FunctionApplicationTest) {
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

TEST(FunctionParsingTest, FunctionWithMultipleArgumentsTest) {
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

TEST(FunctionParsingTest, HighOrderFunctionTEST) {
    Scope ss;
    lexers::Lexer lex;
    lex.appendExp("(define (bar f x) (+ (f x) 1))")
            .appendExp("(define (foo x) (+ x 1))")
            .appendExp("(define n 4)")
            .appendExp("(bar foo n)");

    auto exprPtr = parseAllExpr(lex)->eval(ss);

    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(exprPtr));
    auto numPtr = std::dynamic_pointer_cast<NumberAST>(exprPtr);
    ASSERT_EQ(6, numPtr->getValue());
}

TEST(FunctionParsingTest, LambdaDefintionTest) {
    Scope ss;
    lexers::Lexer lex;
    lex.appendExp("((lambda (x) (+ x 1)) 5)");
    auto exprPtr = parseAllExpr(lex)->eval(ss);

    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(exprPtr));
    auto numPtr = std::dynamic_pointer_cast<NumberAST>(exprPtr);
    ASSERT_EQ(6, numPtr->getValue());
}

TEST(FunctionParsingTest, LambdaApplicationTest) {
    Scope ss;
    lexers::Lexer lex;
    lex.appendExp("(define (bar f y) (+ (f y) 1))")
            .appendExp("(define n 4)")
            .appendExp("(bar (lambda (x) (+ x 2 1)) n)");

    auto exprPtr = parseAllExpr(lex)->eval(ss);

    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(exprPtr));
    auto numPtr = std::dynamic_pointer_cast<NumberAST>(exprPtr);
    ASSERT_EQ(8, numPtr->getValue());
}

TEST(FunctionParsingTest, RecursiveTest) {
    Scope ss;
    lexers::Lexer lex;
    lex.appendExp("(load \"Base.scm\")").appendExp("(define (add x y) (if x (+ 1 (add (- x 1) y)) y))")
            .appendExp("(add 5 6)");

    auto exprPtr = parseAllExpr(lex)->eval(ss);
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(exprPtr));
    auto numPtr = std::dynamic_pointer_cast<NumberAST>(exprPtr);
    ASSERT_EQ(11, numPtr->getValue());

    lex.appendExp("(define (add2 x y) (if x (+ (add2 y (- x 1)) 1) y))")
            .appendExp("(add2 5 6)");
    exprPtr = parseAllExpr(lex)->eval(ss);
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(exprPtr));
    numPtr = std::dynamic_pointer_cast<NumberAST>(exprPtr);
    ASSERT_EQ(11, numPtr->getValue());
}

TEST(FunctionParsingTest, VariableArgumentsTest) {
    Scope ss;
    lexers::Lexer lex;
    lex.appendExp("(define (add-list l) (if (null? l) 0 (+ (car l) (add-list (cdr l)))))")
            .appendExp("(add-list (list 5 6 7 8))");
    auto exprPtr = parseAllExpr(lex)->eval(ss);
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(exprPtr));
    auto numPtr = std::dynamic_pointer_cast<NumberAST>(exprPtr);
    ASSERT_EQ(26, numPtr->getValue());

    lex.appendExp("(define (add x . args) (+ (add-list args) x))")
            .appendExp("(add 5 6 7 8)");
    exprPtr = parseAllExpr(lex)->eval(ss);
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(exprPtr));
    numPtr = std::dynamic_pointer_cast<NumberAST>(exprPtr);
    ASSERT_EQ(26, numPtr->getValue());
}
