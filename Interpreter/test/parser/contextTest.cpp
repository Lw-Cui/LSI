#include <memory>
#include <gtest/gtest.h>
#include <parser.h>
#include <testMacro.h>

using namespace lexers;
using namespace parser;
using namespace std;

TEST(ContextTest, MultipleDefineTest) {
    CREATE_CONTEXT();
    lex.appendExp("(define (add-and-plus2 x y) (add (add x y) 2))"
                      "  (define (add x y) (+ x y))");

    REPL_COND("(add-and-plus2 5 6)", TO_NUM_PTR(res));
    ASSERT_EQ(13, numPtr->getValue());
    ASSERT_STREQ("13", disp.to_string().c_str());
}

TEST(ContextTest, ScopeTest) {
    CREATE_CONTEXT();
    lex.appendExp("(define (Test_driver)"
                      "  (define a 0)"
                      "  (let ((b 0)) (+ a b)))"
                      "(define a 1)"
                      "(define b 2)");

    REPL_COND("(Test_driver)", TO_NUM_PTR(res));
    ASSERT_EQ(0, numPtr->getValue());
    ASSERT_STREQ("0", disp.to_string().c_str());
}
