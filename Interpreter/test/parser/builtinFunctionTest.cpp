#include <memory>
#include <gtest/gtest.h>
#include <parser.h>
#include <testMacro.h>

using namespace lexers;
using namespace parser;


TEST(BuiltinFunctionTest, BuiltinConsTest) {
    CREATE_CONTEXT();
    lex.appendExp("(define p (cons 1 2))");
    REPL_COND("(cdr p)", TO_NUM_PTR(res));
    ASSERT_EQ(2, numPtr->getValue());
    ASSERT_STREQ("2", disp.to_string().c_str());

    REPL_COND("(car p)", TO_NUM_PTR(res));
    ASSERT_EQ(1, numPtr->getValue());
    ASSERT_STREQ("1", disp.to_string().c_str());

    lex.appendExp("(define pp (cons 3 p))");
    REPL_COND("(car (cdr pp))", TO_NUM_PTR(res));
    ASSERT_EQ(1, numPtr->getValue());
    ASSERT_STREQ("1", disp.to_string().c_str());
}

TEST(BuiltinFunctionTest, NullTest) {
    CREATE_CONTEXT();
    lex.appendExp("(define p (cons 1 nil))");

    REPL_COND("(null? (cdr p))", TO_TRUE_PTR(res));
    ASSERT_STREQ("#t", disp.to_string().c_str());

    REPL_COND("(null? (car p))", TO_FALSE_PTR(res));
    ASSERT_STREQ("#f", disp.to_string().c_str());
}

TEST(BuiltinFunctionTest, AddTest) {
    CREATE_CONTEXT();
    REPL_COND("(+ 5 6 7)", TO_NUM_PTR(res));
    ASSERT_EQ(18, numPtr->getValue());
    ASSERT_STREQ("18", disp.to_string().c_str());
}

TEST(BuiltinFunctionTest, MultiplyTest) {
    CREATE_CONTEXT();
    REPL_COND("(* 5 6 7)", TO_NUM_PTR(res));
    ASSERT_EQ(210, numPtr->getValue());
    ASSERT_STREQ("210", disp.to_string().c_str());
}

TEST(BuiltinFunctionTest, MinusTest) {
    CREATE_CONTEXT();
    lex.appendExp("(load \"setup.scm\")");
    REPL_COND("(- 5)", TO_NUM_PTR(res));
    ASSERT_EQ(-5, numPtr->getValue());
    ASSERT_STREQ("-5", disp.to_string().c_str());

    REPL_COND("(+ (- 8) 6 (+ 1))", TO_NUM_PTR(res));
    ASSERT_EQ(-1, numPtr->getValue());
    ASSERT_STREQ("-1", disp.to_string().c_str());
}

TEST(BuiltinFunctionTest, LessThanTest) {
    CREATE_CONTEXT();
    REPL_COND("(< 5 6 7)", TO_TRUE_PTR(res));
    ASSERT_STREQ("#t", disp.to_string().c_str());

    lex.appendExp("(define n 6)");
    REPL_COND("(< n 5)", TO_FALSE_PTR(res));
    ASSERT_STREQ("#f", disp.to_string().c_str());

    REPL_COND("(< 0 0)", TO_FALSE_PTR(res));
    ASSERT_STREQ("#f", disp.to_string().c_str());
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
    ASSERT_STREQ("1", disp.to_string().c_str());

    REPL_COND("(car (cdr seq))", TO_NUM_PTR(res));
    ASSERT_EQ(2, numPtr->getValue());
    ASSERT_STREQ("2", disp.to_string().c_str());

    REPL_COND("(null? (cdr (cdr (cdr seq))))", TO_TRUE_PTR(res));

    lex.appendExp("(define aseq (list 5 seq 6))");
    REPL_COND("(car (cdr (car (cdr aseq))))", TO_NUM_PTR(res));
    ASSERT_EQ(2, numPtr->getValue());
    ASSERT_STREQ("2", disp.to_string().c_str());
}

