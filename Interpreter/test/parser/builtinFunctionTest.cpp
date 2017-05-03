#include <memory>
#include <gtest/gtest.h>
#include <parser.h>

using namespace lexers;
using namespace parser;

TEST(BuiltinFunctionTest, BuiltinConsTest) {
    auto s = std::make_shared<Scope>();
    lexers::Lexer lex("(define p (cons 1 2))");

    lex.appendExp("(cdr p)");
    auto ast = parseAllExpr(lex);
    auto res = ast->eval(s, ast);
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(res));
    auto numPtr = std::dynamic_pointer_cast<NumberAST>(res);
    ASSERT_EQ(2, numPtr->getValue());
    ASSERT_STREQ("2", numPtr->display().c_str());

    lex.appendExp("(car p)");
    ast = parseAllExpr(lex);
    res = ast->eval(s, ast);
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(res));
    numPtr = std::dynamic_pointer_cast<NumberAST>(res);
    ASSERT_EQ(1, numPtr->getValue());
    ASSERT_STREQ("1", numPtr->display().c_str());

    lex.appendExp("(define pp (cons 3 p))")
            .appendExp("(car (cdr pp))");
    ast = parseAllExpr(lex);
    res = ast->eval(s, ast);
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(res));
    numPtr = std::dynamic_pointer_cast<NumberAST>(res);
    ASSERT_EQ(1, numPtr->getValue());
    ASSERT_STREQ("1", numPtr->display().c_str());
}

TEST(BuiltinFunctionTest, NullTest) {
    auto s = std::make_shared<Scope>();
    lexers::Lexer lex("(define p (cons 1 nil))");

    lex.appendExp("(null? (cdr p))");
    auto ast = parseAllExpr(lex);
    auto res = ast->eval(s, ast);
    ASSERT_TRUE(std::dynamic_pointer_cast<BooleansTrueAST>(res));
    auto trueBool = std::dynamic_pointer_cast<BooleansTrueAST>(res);
    ASSERT_STREQ("#t", trueBool->display().c_str());

    lex.appendExp("(null? (car p))");
    ast = parseAllExpr(lex);
    res = ast->eval(s, ast);
    ASSERT_TRUE(std::dynamic_pointer_cast<BooleansFalseAST>(res));
    auto falseBool = std::dynamic_pointer_cast<BooleansFalseAST>(res);
    ASSERT_STREQ("#f", falseBool->display().c_str());
}

TEST(BuiltinFunctionTest, AddTest) {
    auto ss = std::make_shared<Scope>();
    lexers::Lexer lex{"(+ 5 6 7)"};
    auto ast = parseExpr(lex);
    auto exprPtr = ast->eval(ss, ast);
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(exprPtr));
    auto numPtr = std::dynamic_pointer_cast<NumberAST>(exprPtr);
    ASSERT_EQ(18, numPtr->getValue());
    ASSERT_STREQ("18", numPtr->display().c_str());
}

TEST(BuiltinFunctionTest, MultiplyTest) {
    auto ss = std::make_shared<Scope>();
    lexers::Lexer lex{"(* 5 6 7)"};
    auto ast = parseExpr(lex);
    auto exprPtr = ast->eval(ss, ast);
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(exprPtr));
    auto numPtr = std::dynamic_pointer_cast<NumberAST>(exprPtr);
    ASSERT_EQ(210, numPtr->getValue());
    ASSERT_STREQ("210", numPtr->display().c_str());
}

TEST(BuiltinFunctionTest, MinusTest) {
    auto ss = std::make_shared<Scope>();

    lexers::Lexer lex;
    lex.appendExp("(load \"setup.scm\")").appendExp("(- 5)");
    auto ast = parseAllExpr(lex);
    auto exprPtr = ast->eval(ss, ast);
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(exprPtr));
    auto numPtr = std::dynamic_pointer_cast<NumberAST>(exprPtr);
    ASSERT_EQ(-5, numPtr->getValue());
    ASSERT_STREQ("-5", numPtr->display().c_str());

    lex.appendExp("(+ (- 8) 6 (+ 1))");
    ast = parseExpr(lex);
    exprPtr = ast->eval(ss, ast);
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(exprPtr));
    numPtr = std::dynamic_pointer_cast<NumberAST>(exprPtr);
    ASSERT_EQ(-1, numPtr->getValue());
    ASSERT_STREQ("-1", numPtr->display().c_str());
}

TEST(BuiltinFunctionTest, LessThanTest) {
    auto ss = std::make_shared<Scope>();
    lexers::Lexer lex("(< 5 6 7)");
    auto ast = parseExpr(lex);
    auto exprPtr = ast->eval(ss, ast);
    ASSERT_TRUE(std::dynamic_pointer_cast<BooleansTrueAST>(exprPtr));
    auto trueBool = std::dynamic_pointer_cast<BooleansTrueAST>(exprPtr);
    ASSERT_STREQ("#t", trueBool->display().c_str());

    lex.appendExp("(define n 6)");
    ast = parseExpr(lex);
    ast->eval(ss, ast);

    lex.appendExp("(< n 5)");
    ast = parseExpr(lex);
    exprPtr = ast->eval(ss, ast);
    ASSERT_TRUE(std::dynamic_pointer_cast<BooleansFalseAST>(exprPtr));
    auto falseBool = std::dynamic_pointer_cast<BooleansFalseAST>(exprPtr);
    ASSERT_STREQ("#f", falseBool->display().c_str());

    lex.appendExp("(< 0 0)");
    ast = parseAllExpr(lex);
    exprPtr = ast->eval(ss, ast);
    ASSERT_TRUE(std::dynamic_pointer_cast<BooleansFalseAST>(exprPtr));
    falseBool = std::dynamic_pointer_cast<BooleansFalseAST>(exprPtr);
    ASSERT_STREQ("#f", falseBool->display().c_str());

}

TEST(BuiltinFunctionTest, NilTest) {
    auto ss = std::make_shared<Scope>();
    lexers::Lexer lex;
    lex.appendExp("(null? (list 1 2 3))");
    auto ast = parseAllExpr(lex);
    auto exprPtr = ast->eval(ss, ast);
    ASSERT_TRUE(std::dynamic_pointer_cast<BooleansFalseAST>(exprPtr));

    lex.appendExp("(define l (list 1 2 3))").appendExp("(null? l)");
    ast = parseAllExpr(lex);
    exprPtr = ast->eval(ss, ast);
    ASSERT_TRUE(std::dynamic_pointer_cast<BooleansFalseAST>(exprPtr));

    lex.appendExp("(null? nil)");
    ast = parseAllExpr(lex);
    exprPtr = ast->eval(ss, ast);
    ASSERT_TRUE(std::dynamic_pointer_cast<BooleansTrueAST>(exprPtr));

    lex.appendExp("(define (add-list l) (if (null? l) #t #f))")
            .appendExp("(add-list (list 5 6 7 8))");
    ast = parseAllExpr(lex);
    exprPtr = ast->eval(ss, ast);
    ASSERT_TRUE(std::dynamic_pointer_cast<BooleansFalseAST>(exprPtr));
}

TEST(BuiltinFunctionTest, ListTest) {
    auto ss = std::make_shared<Scope>();
    lexers::Lexer lex;
    lex.appendExp("(define seq (list 1 2 3))").appendExp("(car seq)");
    auto ast = parseAllExpr(lex);
    auto exprPtr = ast->eval(ss, ast);
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(exprPtr));
    auto numPtr = std::dynamic_pointer_cast<NumberAST>(exprPtr);
    ASSERT_EQ(1, numPtr->getValue());
    ASSERT_STREQ("1", numPtr->display().c_str());

    lex.appendExp("(car (cdr seq))");
    ast = parseAllExpr(lex);
    exprPtr = ast->eval(ss, ast);
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(exprPtr));
    numPtr = std::dynamic_pointer_cast<NumberAST>(exprPtr);
    ASSERT_EQ(2, numPtr->getValue());
    ASSERT_STREQ("2", numPtr->display().c_str());

    lex.appendExp("(null? (cdr (cdr (cdr seq))))");
    ast = parseAllExpr(lex);
    exprPtr = ast->eval(ss, ast);
    ASSERT_TRUE(std::dynamic_pointer_cast<BooleansTrueAST>(exprPtr));

    lex.appendExp("(define aseq (list 5 seq 6))").appendExp("(car (cdr (car (cdr aseq))))");
    ast = parseAllExpr(lex);
    exprPtr = ast->eval(ss, ast);
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(exprPtr));
    numPtr = std::dynamic_pointer_cast<NumberAST>(exprPtr);
    ASSERT_EQ(2, numPtr->getValue());
    ASSERT_STREQ("2", numPtr->display().c_str());
}

