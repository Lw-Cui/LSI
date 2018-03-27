#include <memory>
#include <gtest/gtest.h>
#include <parser.h>
#include <testMacro.h>

using namespace lexers;
using namespace parser;
using namespace std;

TEST(AdvancedFuncTest, LambdaFunctionRecursiveTest) {
    CREATE_CONTEXT();
    lex.appendExp("(load \"setup.scm\")");
    REPL_COND("(((lambda (give-me-a-function)"
                      "   ((lambda (f) (f f))"
                      "    (lambda (fact-function)"
                      "      (give-me-a-function"
                      "       (lambda (x) ((fact-function fact-function) x))))))"
                      " (lambda (graceful-fact-function)"
                      "   (lambda (x)"
                      "     (if (= x 0)"
                      "         1"
                      "         (* x (graceful-fact-function (- x 1))))))) 5)", TO_NUM_PTR(res));
    ASSERT_EQ(120, numPtr->getValue());
    ASSERT_STREQ("120", disp.to_string().c_str());
}

TEST(AdvancedFuncTest, LambdaFunctionRecursiveTestV2) {
    CREATE_CONTEXT();
    lex.appendExp("(load \"setup.scm\")");
    REPL_COND("(((lambda (fact-function-2)"
                      " (lambda (x)"
                      "   (if (= x 0)"
                      "       1"
                      "       (* x ((fact-function-2 fact-function-2) (- x 1))))))"
                      " (lambda (fact-function-2)"
                      "   (lambda (x)"
                      "     (if (= x 0)"
                      "         1"
                      "         (* x ((fact-function-2 fact-function-2) (- x 1))))))) 5)", TO_NUM_PTR(res));
    ASSERT_EQ(120, numPtr->getValue());
    ASSERT_STREQ("120", disp.to_string().c_str());
}

TEST(AdvancedFuncTest, LambdaFunctionRecursiveTestV3) {
    CREATE_CONTEXT();
    lex.appendExp("(load \"setup.scm\")");
    REPL_COND("(((lambda (f) (f f))"
                      " (lambda (fact-function-3)"
                      "   (lambda (x)"
                      "     (if (= x 0)"
                      "         1"
                      "         (* x ((fact-function-3 fact-function-3) (- x 1))))))) 5)", TO_NUM_PTR(res));
    ASSERT_EQ(120, numPtr->getValue());
    ASSERT_STREQ("120", disp.to_string().c_str());
}

TEST(AdvancedFuncTest, LambdaFunctionRecursiveTestV4) {
    CREATE_CONTEXT();
    lex.appendExp("(load \"setup.scm\")");
    REPL_COND("(((lambda (f) (f f))"
                      " (lambda (fact-function-4)"
                      "   ((lambda (graceful-fact-function)"
                      "      (lambda (x)"
                      "        (if (= x 0)"
                      "            1"
                      "            (* x (graceful-fact-function (- x 1))))))"
                      "    (lambda (x) ((fact-function-4 fact-function-4) x))))) 5)", TO_NUM_PTR(res));
    ASSERT_EQ(120, numPtr->getValue());
    ASSERT_STREQ("120", disp.to_string().c_str());
}

TEST(AdvancedFuncTest, RecursiveTest) {
    CREATE_CONTEXT();
    lex.appendExp("(load \"setup.scm\")").appendExp("(define (add x y) (if (not (= x 0)) (+ 1 (add (- x 1) y)) y))");
    REPL_COND("(add 5 6)", TO_NUM_PTR(res));
    ASSERT_EQ(11, numPtr->getValue());
    ASSERT_STREQ("11", disp.to_string().c_str());

    lex.appendExp("(define (add2 x y) (if (not (= x 0)) (+ (add2 y (- x 1)) 1) y))");
    REPL_COND("(add2 5 6)", TO_NUM_PTR(res));
    ASSERT_EQ(11, numPtr->getValue());
    ASSERT_STREQ("11", disp.to_string().c_str());
}
