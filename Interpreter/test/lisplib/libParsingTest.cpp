#include <memory>
#include <gtest/gtest.h>
#include <parser.h>
#include <exception.h>
#include <testMacro.h>
#include <visitor.h>

using namespace lexers;
using namespace parser;
using namespace exception;

TEST(LibrariesParsingTest, BasicConsTest) {
    try {
        CREATE_CONTEXT();
        lex.appendExp("(load \"setup.scm\")");
        REPL_COND("(load \"Test.scm\")", s->count("Cons"));
        ASSERT_TRUE(s->count("Car"));
        ASSERT_TRUE(s->count("Cdr"));

        lex.appendExp("(define p (Cons 1 2))");
        REPL_COND("(Cdr p)", TO_NUM_PTR(res));
        ASSERT_EQ(2, numPtr->getValue());

        REPL_COND("(Car p)", TO_NUM_PTR(res));
        ASSERT_EQ(1, numPtr->getValue());
    } catch (RuntimeError &e) {
        CLOG(DEBUG, "exception") << e.what();
        throw;
    }
}

TEST(LibrariesParsingTest, AdvanceConsTest) {
    try {
        CREATE_CONTEXT();
        lex.appendExp("(load \"setup.scm\")")
                .appendExp("(load \"Test.scm\")")
                .appendExp("(define p (Cons 1 2))")
                .appendExp("(define pp (Cons 3 p))");
        REPL_COND("(Car (Cdr pp))", TO_NUM_PTR(res));
        ASSERT_EQ(1, numPtr->getValue());
    } catch (RuntimeError &e) {
        CLOG(DEBUG, "exception") << e.what();
        throw;
    }
}

