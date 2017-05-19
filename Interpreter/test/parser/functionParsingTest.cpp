#include <memory>
#include <gtest/gtest.h>
#include <parser.h>
#include <testMacro.h>

using namespace lexers;
using namespace parser;
using namespace std;

TEST(FunctionParsingTest, FunctionDefinitionTest) {
    CREATE_CONTEXT();
    REPL_COND("(define (foo x) x)", s->count("foo"));
    ASSERT_STREQ("#proceduce", s->searchName("foo")->display().c_str());
}

TEST(FunctionParsingTest, FunctionApplicationTest) {
    CREATE_CONTEXT();
    REPL_COND("(define (foo x) x)", s->count("foo"));
    ASSERT_STREQ("#proceduce", s->searchName("foo")->display().c_str());

    REPL_COND("(foo 5)", TO_NUM_PTR(res));
    ASSERT_EQ(5, numPtr->getValue());

    REPL_COND("(define (bar x) (+ x 1))", s->count("bar"));
    ASSERT_STREQ("#proceduce", s->searchName("bar")->display().c_str());

    REPL_COND("(bar 4)", TO_NUM_PTR(res));
    ASSERT_EQ(5, numPtr->getValue());

    REPL_COND("(define n 4)", s->count("n"));

    REPL_COND("(bar n)", TO_NUM_PTR(res));
    ASSERT_EQ(5, numPtr->getValue());

    REPL_COND("(define b (foo (bar n)))", s->count("b"));
    res = s->searchName("b");
    ASSERT_TRUE(TO_NUM_PTR(res));
    numPtr = TO_NUM_PTR(res);
    ASSERT_EQ(5, numPtr->getValue());
}

TEST(FunctionParsingTest, FunctionWithMultipleArgumentsTest) {
    CREATE_CONTEXT();
    REPL_COND("(define (bar x y) (+ x y 1))", s->count("bar"));
    REPL_COND("(define n 5)", true);
    REPL_COND("(bar n 4)", TO_NUM_PTR(res));
    ASSERT_EQ(10, numPtr->getValue());
}

TEST(FunctionParsingTest, HighOrderFunctionTEST) {
    CREATE_CONTEXT();

    lex.appendExp("(define (bar f x) (+ (f x) 1))")
            .appendExp("(define (foo x) (+ x 1))")
            .appendExp("(define n 4)");

    REPL_COND("(bar foo n)", TO_NUM_PTR(res));
    ASSERT_EQ(6, numPtr->getValue());
}

TEST(FunctionParsingTest, LambdaDefintionTest) {
    CREATE_CONTEXT();
    REPL_COND("((lambda (x) (+ x 1)) 5)", TO_NUM_PTR(res));
    ASSERT_EQ(6, numPtr->getValue());
    ASSERT_STREQ("6", numPtr->display().c_str());
}

TEST(FunctionParsingTest, LambdaApplicationTest) {
    CREATE_CONTEXT();
    lex.appendExp("(define (bar f y) (+ (f y) 1))")
            .appendExp("(define n 4)");

    REPL_COND("(bar (lambda (x) (+ x 2 1)) n)", TO_NUM_PTR(res));
    ASSERT_EQ(8, numPtr->getValue());
}

TEST(FunctionParsingTest, RecursiveTest) {
    CREATE_CONTEXT();
    lex.appendExp("(load \"setup.scm\")").appendExp("(define (add x y) (if (not (= x 0)) (+ 1 (add (- x 1) y)) y))");
    REPL_COND("(add 5 6)", TO_NUM_PTR(res));
    ASSERT_EQ(11, numPtr->getValue());
    ASSERT_STREQ("11", numPtr->display().c_str());

    lex.appendExp("(define (add2 x y) (if (not (= x 0)) (+ (add2 y (- x 1)) 1) y))");
    REPL_COND("(add2 5 6)", TO_NUM_PTR(res));
    ASSERT_EQ(11, numPtr->getValue());
    ASSERT_STREQ("11", numPtr->display().c_str());
}

TEST(FunctionParsingTest, VariableArgumentsTest) {
    CREATE_CONTEXT();
    lex.appendExp("(define (add-list l) (if (null? l) 0 (+ (car l) (add-list (cdr l)))))");
    REPL_COND("(add-list (list 5 6 7 8))", TO_NUM_PTR(res));
    ASSERT_EQ(26, numPtr->getValue());
    ASSERT_STREQ("26", numPtr->display().c_str());

    lex.appendExp("(define (add x . args) (+ (add-list args) x))");
    REPL_COND("(add 5 6 7 8)", TO_NUM_PTR(res));
    ASSERT_EQ(26, numPtr->getValue());
    ASSERT_STREQ("26", numPtr->display().c_str());
}

TEST(FunctionParsingTest, MultipleExpressionTest) {
    CREATE_CONTEXT();
    lex.appendExp("(define (getNum) (+ 5 6) (+ 7 8))");
    REPL_COND("(getNum)", TO_NUM_PTR(res));
    ASSERT_EQ(15, numPtr->getValue());
    ASSERT_STREQ("15", numPtr->display().c_str());

    lex.appendExp("(define (getNumV2)"
                          "  (define (add-and-plus2 x y) (add (add x y) 2))"
                          "  (define (add x y) (+ x y))"
                          "  (add 7 8)"
                          "  (add-and-plus2 5 6))");

    REPL_COND("(getNumV2)", TO_NUM_PTR(res));
    ASSERT_EQ(13, numPtr->getValue());
    ASSERT_STREQ("13", numPtr->display().c_str());
}

TEST(FunctionParsingTest, NestedFunctionTest) {
    CREATE_CONTEXT();
    lex.appendExp("(define (and expr . args)"
                          "(define (list-and l)"
                          "(if (null? l) #t (if (car l) (list-and (cdr l)) #f)))"
                          "(if expr (list-and args) #f))");
    REPL_COND("(and 5 6 7 8)", TO_TRUE_PTR(res));
}

TEST(FunctionParsingTest, LambdaFunctionRecursiveTest) {
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
    ASSERT_STREQ("120", numPtr->display().c_str());
}

TEST(FunctionParsingTest, LambdaFunctionRecursiveTestV2) {
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
    ASSERT_STREQ("120", numPtr->display().c_str());
}

TEST(FunctionParsingTest, LambdaFunctionRecursiveTestV3) {
    CREATE_CONTEXT();
    lex.appendExp("(load \"setup.scm\")");
    REPL_COND("(((lambda (f) (f f))"
                      " (lambda (fact-function-3)"
                      "   (lambda (x)"
                      "     (if (= x 0)"
                      "         1"
                      "         (* x ((fact-function-3 fact-function-3) (- x 1))))))) 5)", TO_NUM_PTR(res));
    ASSERT_EQ(120, numPtr->getValue());
    ASSERT_STREQ("120", numPtr->display().c_str());
}

TEST(FunctionParsingTest, LambdaFunctionRecursiveTestV4) {
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
    ASSERT_STREQ("120", numPtr->display().c_str());
}
