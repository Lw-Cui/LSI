#include <memory>
#include <gtest/gtest.h>
#include <parser.h>

using namespace lexers;
using namespace parser;

#define CREATE_CONTEXT() \
    auto s = std::make_shared<Scope>();\
    lexers::Lexer lex;\
    pExpr ast, res;\
    std::shared_ptr<NumberAST> numPtr;\
    std::shared_ptr<BooleansTrueAST> trueBool;\
    std::shared_ptr<BooleansFalseAST> falseBool;

#define TO_NUM_PTR(res)\
    std::dynamic_pointer_cast<NumberAST>(res)

#define TO_TRUE_PTR(res)\
    std::dynamic_pointer_cast<BooleansTrueAST>(res)

#define TO_FALSE_PTR(res)\
    std::dynamic_pointer_cast<BooleansFalseAST>(res)

#define REPL_COND(str, condition)\
    lex.appendExp(str);\
    ast = parseAllExpr(lex);\
    res = ast->eval(s, ast);\
    ASSERT_TRUE(condition);\
    numPtr = TO_NUM_PTR(res);\
    trueBool = TO_TRUE_PTR(res);\
    falseBool = TO_FALSE_PTR(res);


TEST(BuiltinFunctionTest, BuiltinConsTest) {
    CREATE_CONTEXT();
    lex.appendExp("(define p (cons 1 2))");
    REPL_COND("(cdr p)", TO_NUM_PTR(res));
    ASSERT_EQ(2, numPtr->getValue());
    ASSERT_STREQ("2", numPtr->display().c_str());

    REPL_COND("(car p)", TO_NUM_PTR(res));
    ASSERT_EQ(1, numPtr->getValue());
    ASSERT_STREQ("1", numPtr->display().c_str());

    lex.appendExp("(define pp (cons 3 p))");
    REPL_COND("(car (cdr pp))", TO_NUM_PTR(res));
    ASSERT_EQ(1, numPtr->getValue());
    ASSERT_STREQ("1", numPtr->display().c_str());
}

TEST(BuiltinFunctionTest, NullTest) {
    CREATE_CONTEXT();
    lex.appendExp("(define p (cons 1 nil))");

    REPL_COND("(null? (cdr p))", TO_TRUE_PTR(res));
    ASSERT_STREQ("#t", trueBool->display().c_str());

    REPL_COND("(null? (car p))", TO_FALSE_PTR(res));
    ASSERT_STREQ("#f", falseBool->display().c_str());
}

TEST(BuiltinFunctionTest, AddTest) {
    CREATE_CONTEXT();
    REPL_COND("(+ 5 6 7)", TO_NUM_PTR(res));
    ASSERT_EQ(18, numPtr->getValue());
    ASSERT_STREQ("18", numPtr->display().c_str());
}

TEST(BuiltinFunctionTest, MultiplyTest) {
    CREATE_CONTEXT();
    REPL_COND("(* 5 6 7)", TO_NUM_PTR(res));
    ASSERT_EQ(210, numPtr->getValue());
    ASSERT_STREQ("210", numPtr->display().c_str());
}

TEST(BuiltinFunctionTest, MinusTest) {
    CREATE_CONTEXT();
    lex.appendExp("(load \"setup.scm\")");
    REPL_COND("(- 5)", TO_NUM_PTR(res));
    ASSERT_EQ(-5, numPtr->getValue());
    ASSERT_STREQ("-5", numPtr->display().c_str());

    REPL_COND("(+ (- 8) 6 (+ 1))", TO_NUM_PTR(res));
    ASSERT_EQ(-1, numPtr->getValue());
    ASSERT_STREQ("-1", numPtr->display().c_str());
}

TEST(BuiltinFunctionTest, LessThanTest) {
    CREATE_CONTEXT();
    REPL_COND("(< 5 6 7)", TO_TRUE_PTR(res));
    ASSERT_STREQ("#t", trueBool->display().c_str());

    lex.appendExp("(define n 6)");
    REPL_COND("(< n 5)", TO_FALSE_PTR(res));
    ASSERT_STREQ("#f", falseBool->display().c_str());

    REPL_COND("(< 0 0)", TO_FALSE_PTR(res));
    ASSERT_STREQ("#f", falseBool->display().c_str());
}

TEST(BuiltinFunctionTest, NilTest) {
    CREATE_CONTEXT();
    REPL_COND("(null? (list 1 2 3))", TO_FALSE_PTR(res));

    lex.appendExp("(define l (list 1 2 3))");
    REPL_COND("(null? l)", TO_FALSE_PTR(res));

    lex.appendExp("(define (add-list l) (if (null? l) #t #f))");
    REPL_COND("(add-list (list 5 6 7 8))", TO_FALSE_PTR(res));
}

TEST(BuiltinFunctionTest, ListTest) {
    CREATE_CONTEXT();
    lex.appendExp("(define seq (list 1 2 3))");
    REPL_COND("(car seq)", TO_NUM_PTR(res));
    ASSERT_EQ(1, numPtr->getValue());
    ASSERT_STREQ("1", numPtr->display().c_str());

    REPL_COND("(car (cdr seq))", TO_NUM_PTR(res));
    ASSERT_EQ(2, numPtr->getValue());
    ASSERT_STREQ("2", numPtr->display().c_str());

    REPL_COND("(null? (cdr (cdr (cdr seq))))", TO_TRUE_PTR(res));

    lex.appendExp("(define aseq (list 5 seq 6))");
    REPL_COND("(car (cdr (car (cdr aseq))))", TO_NUM_PTR(res));
    ASSERT_EQ(2, numPtr->getValue());
    ASSERT_STREQ("2", numPtr->display().c_str());
}

