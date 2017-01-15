#include <memory>
#include <gtest/gtest.h>
#include <parser.h>

using namespace lexers;
using namespace parser;

TEST(BuiltinFunctionTest, BuiltinConsTest) {
    Scope s;
    lexers::Lexer lex("(define p (cons 1 2))");

    lex.appendExp("(cdr p)");
    auto res = parseAllExpr(lex)->eval(s);
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(res));
    auto numPtr = std::dynamic_pointer_cast<NumberAST>(res);
    ASSERT_EQ(2, numPtr->getValue());
    ASSERT_STREQ("2", numPtr->display().c_str());

    lex.appendExp("(car p)");
    res = parseAllExpr(lex)->eval(s);
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(res));
    numPtr = std::dynamic_pointer_cast<NumberAST>(res);
    ASSERT_EQ(1, numPtr->getValue());
    ASSERT_STREQ("1", numPtr->display().c_str());

    lex.appendExp("(define pp (cons 3 p))")
            .appendExp("(car (cdr pp))");
    res = parseAllExpr(lex)->eval(s);
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(res));
    numPtr = std::dynamic_pointer_cast<NumberAST>(res);
    ASSERT_EQ(1, numPtr->getValue());
    ASSERT_STREQ("1", numPtr->display().c_str());
}

TEST(BuiltinFunctionTest, NullTest) {
    Scope s;
    lexers::Lexer lex("(define p (cons 1 nil))");

    lex.appendExp("(null? (cdr p))");
    auto res = parseAllExpr(lex)->eval(s);
    ASSERT_TRUE(std::dynamic_pointer_cast<BooleansTrueAST>(res));
    auto trueBool = std::dynamic_pointer_cast<BooleansTrueAST>(res);
    ASSERT_STREQ("#t", trueBool->display().c_str());

    lex.appendExp("(null? (car p))");
    res = parseAllExpr(lex)->eval(s);
    ASSERT_TRUE(std::dynamic_pointer_cast<BooleansFalseAST>(res));
    auto falseBool = std::dynamic_pointer_cast<BooleansFalseAST>(res);
    ASSERT_STREQ("#f", falseBool->display().c_str());
}

TEST(BuiltinFunctionTest, AddTest) {
    Scope ss;
    lexers::Lexer lex{"(+ 5 6 7)"};
    auto exprPtr = parseExpr(lex)->eval(ss);
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(exprPtr));
    auto numPtr = std::dynamic_pointer_cast<NumberAST>(exprPtr);
    ASSERT_EQ(18, numPtr->getValue());
    ASSERT_STREQ("18", numPtr->display().c_str());
}

TEST(BuiltinFunctionTest, MultiplyTest) {
    Scope ss;
    lexers::Lexer lex{"(* 5 6 7)"};
    auto exprPtr = parseExpr(lex)->eval(ss);
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(exprPtr));
    auto numPtr = std::dynamic_pointer_cast<NumberAST>(exprPtr);
    ASSERT_EQ(210, numPtr->getValue());
    ASSERT_STREQ("210", numPtr->display().c_str());
}

TEST(BuiltinFunctionTest, MinusTest) {
    Scope ss;

    lexers::Lexer lex;
    lex.appendExp("(load \"setup.scm\")").appendExp("(- 5)");
    auto exprPtr = parseAllExpr(lex)->eval(ss);
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(exprPtr));
    auto numPtr = std::dynamic_pointer_cast<NumberAST>(exprPtr);
    ASSERT_EQ(-5, numPtr->getValue());
    ASSERT_STREQ("-5", numPtr->display().c_str());

    lex.appendExp("(+ (- 8) 6 (+ 1))");
    exprPtr = parseExpr(lex)->eval(ss);
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(exprPtr));
    numPtr = std::dynamic_pointer_cast<NumberAST>(exprPtr);
    ASSERT_EQ(-1, numPtr->getValue());
    ASSERT_STREQ("-1", numPtr->display().c_str());
}

TEST(BuiltinFunctionTest, LessThanTest) {
    Scope ss;
    lexers::Lexer lex("(< 5 6 7)");
    auto exprPtr = parseExpr(lex)->eval(ss);
    ASSERT_TRUE(std::dynamic_pointer_cast<BooleansTrueAST>(exprPtr));
    auto trueBool = std::dynamic_pointer_cast<BooleansTrueAST>(exprPtr);
    ASSERT_STREQ("#t", trueBool->display().c_str());

    lex.appendExp("(define n 6)");
    parseExpr(lex)->eval(ss);

    lex.appendExp("(< n 5)");
    exprPtr = parseExpr(lex)->eval(ss);
    ASSERT_TRUE(std::dynamic_pointer_cast<BooleansFalseAST>(exprPtr));
    auto falseBool = std::dynamic_pointer_cast<BooleansFalseAST>(exprPtr);
    ASSERT_STREQ("#f", falseBool->display().c_str());

    lex.appendExp("(< 0 0)");
    exprPtr = parseAllExpr(lex)->eval(ss);
    ASSERT_TRUE(std::dynamic_pointer_cast<BooleansFalseAST>(exprPtr));
    falseBool = std::dynamic_pointer_cast<BooleansFalseAST>(exprPtr);
    ASSERT_STREQ("#f", falseBool->display().c_str());

}

TEST(BuiltinFunctionTest, NilTest) {
    Scope ss;
    lexers::Lexer lex;
    lex.appendExp("(null? (list 1 2 3))");
    auto exprPtr = parseAllExpr(lex)->eval(ss);
    ASSERT_TRUE(std::dynamic_pointer_cast<BooleansFalseAST>(exprPtr));

    lex.appendExp("(define l (list 1 2 3))").appendExp("(null? l)");
    exprPtr = parseAllExpr(lex)->eval(ss);
    ASSERT_TRUE(std::dynamic_pointer_cast<BooleansFalseAST>(exprPtr));

    lex.appendExp("(null? nil)");
    exprPtr = parseAllExpr(lex)->eval(ss);
    ASSERT_TRUE(std::dynamic_pointer_cast<BooleansTrueAST>(exprPtr));

    lex.appendExp("(define (add-list l) (if (null? l) #t #f))")
            .appendExp("(add-list (list 5 6 7 8))");
    exprPtr = parseAllExpr(lex)->eval(ss);
    ASSERT_TRUE(std::dynamic_pointer_cast<BooleansFalseAST>(exprPtr));
}

TEST(BuiltinFunctionTest, ListTest) {
    Scope ss;
    lexers::Lexer lex;
    lex.appendExp("(define seq (list 1 2 3))").appendExp("(car seq)");
    auto exprPtr = parseAllExpr(lex)->eval(ss);
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(exprPtr));
    auto numPtr = std::dynamic_pointer_cast<NumberAST>(exprPtr);
    ASSERT_EQ(1, numPtr->getValue());
    ASSERT_STREQ("1", numPtr->display().c_str());

    lex.appendExp("(car (cdr seq))");
    exprPtr = parseAllExpr(lex)->eval(ss);
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(exprPtr));
    numPtr = std::dynamic_pointer_cast<NumberAST>(exprPtr);
    ASSERT_EQ(2, numPtr->getValue());
    ASSERT_STREQ("2", numPtr->display().c_str());

    lex.appendExp("(null? (cdr (cdr (cdr seq))))");
    exprPtr = parseAllExpr(lex)->eval(ss);
    ASSERT_TRUE(std::dynamic_pointer_cast<BooleansTrueAST>(exprPtr));

    lex.appendExp("(define aseq (list 5 seq 6))").appendExp("(car (cdr (car (cdr aseq))))");
    exprPtr = parseAllExpr(lex)->eval(ss);
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(exprPtr));
    numPtr = std::dynamic_pointer_cast<NumberAST>(exprPtr);
    ASSERT_EQ(2, numPtr->getValue());
    ASSERT_STREQ("2", numPtr->display().c_str());
}

