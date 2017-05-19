#include <memory>
#include <gtest/gtest.h>
#include <exception.h>
#include <parser.h>
#include <testMacro.h>

using namespace lexers;
using namespace parser;
using namespace exception;

TEST(KeywordParsingTest, IdentifierDefinitionTest) {
    CREATE_CONTEXT();
    REPL_COND("(define n 5)", s->count("n"));
    res = s->searchName("n")->eval(s, s->searchName("n"));
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(res));
    numPtr = std::dynamic_pointer_cast<NumberAST>(res);
    ASSERT_EQ(5, numPtr->getValue());
    ASSERT_EQ(Lexer::TokEOF, lex.getTokType());

    REPL_COND("(define a n)", s->count("a"));

    res = s->searchName("a")->eval(s, s->searchName("a"));
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(res));
    numPtr = std::dynamic_pointer_cast<NumberAST>(res);
    ASSERT_EQ(5, numPtr->getValue());
    ASSERT_EQ(Lexer::TokEOF, lex.getTokType());
}

TEST(KeywordParsingTest, IfStatementTest) {
    CREATE_CONTEXT();
    REPL_COND("(if (+ 5 6) 5 6)", TO_NUM_PTR(res));
    ASSERT_EQ(5, numPtr->getValue());

    lex.appendExp("(load \"setup.scm\")").appendExp("(define (add x y) (+ x y))").appendExp("(define (bar x) (+ x 1))");
    REPL_COND("(define (foo x) (+ x 2))", s->count("foo"));

    REPL_COND("((if (= (add 0 0) 1) bar foo) 0)", TO_NUM_PTR(res));
    ASSERT_EQ(2, numPtr->getValue());
}

TEST(KeywordParsingTest, CondStatementTest) {
    CREATE_CONTEXT();
    lex.appendExp("(load \"setup.scm\")");
    REPL_COND("(cond ((= (+ 5 6) 0) 1)"
                      "       ((= (+ 5 (- 4)) 0) 2)"
                      "       (else 3))", TO_NUM_PTR(res));
    ASSERT_EQ(3, numPtr->getValue());
    REPL_COND("(cond ((= (+ 5 6) 0) 1)"
                      "       ((= (+ 5 (- 5)) 0) 2)"
                      "       (else 3))", TO_NUM_PTR(res));
    ASSERT_EQ(2, numPtr->getValue());
}


TEST(KeywordParsingTest, LetStatementTest) {
    CREATE_CONTEXT();
    lex.appendExp("(load \"setup.scm\")");
    REPL_COND("(let((x 5)"
                      "      (y 6)"
                      "      (foo (lambda (x y) (+ x y))))"
                      "  (foo x y))", TO_NUM_PTR(res));
    ASSERT_EQ(11, numPtr->getValue());
}

TEST(KeywordParsingTest, BooleansTest) {
    CREATE_CONTEXT();
    REPL_COND("#t", TO_TRUE_PTR(res));
    REPL_COND("#f", TO_FALSE_PTR(res));
}

TEST(KeywordParsingTest, LoadingFileTest) {
    CREATE_CONTEXT();
    REPL_COND("(load \"Test.scm\")", s->count("foo"));
    ASSERT_TRUE(s->count("add"));

    REPL_COND("(foo 5)", TO_NUM_PTR(res));
    ASSERT_EQ(6, numPtr->getValue());
    lex.appendExp("(define n 5)");
    REPL_COND("(add n 6)", TO_NUM_PTR(res));
    ASSERT_EQ(11, numPtr->getValue());
}

TEST(KeywordParsingTest, NilTest) {
    CREATE_CONTEXT();
    lex.appendExp("(define p (cons 1 nil))");
    REPL_COND("(cdr p)", std::dynamic_pointer_cast<NilAST>(res));
    auto nil = std::dynamic_pointer_cast<NilAST>(res);
    ASSERT_STREQ("\'()", nil->display().c_str());
}

