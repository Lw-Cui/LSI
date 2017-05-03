#include <memory>
#include <gtest/gtest.h>
#include <parser.h>
#include <exception.h>

using namespace lexers;
using namespace parser;
using namespace exception;

TEST(LibrariesParsingTest, BasicConsTest) {
    try {
        auto s = std::make_shared<Scope>();
        lexers::Lexer lex;
        lex.appendExp("(load \"setup.scm\")").appendExp("(load \"Test.scm\")");
        auto ast = parseAllExpr(lex);
        ast->eval(s, ast);
        ASSERT_TRUE(s->count("Cons"));
        ASSERT_TRUE(s->count("Car"));
        ASSERT_TRUE(s->count("Cdr"));

        lex.appendExp("(define p (Cons 1 2))").appendExp("(Cdr p)");
        ast = parseAllExpr(lex);
        auto res = ast->eval(s, ast);
        ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(res));
        auto numPtr = std::dynamic_pointer_cast<NumberAST>(res);
        ASSERT_EQ(2, numPtr->getValue());

        lex.appendExp("(Car p)");
        ast = parseAllExpr(lex);
        res = ast->eval(s, ast);
        ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(res));
        numPtr = std::dynamic_pointer_cast<NumberAST>(res);
        ASSERT_EQ(1, numPtr->getValue());
    } catch (RuntimeError &e) {
        CLOG(DEBUG, "exception") << e.what();
        throw;
    }
}

TEST(LibrariesParsingTest, AdvanceConsTest) {
    try {
        auto s = std::make_shared<Scope>();
        lexers::Lexer lex;

        lex.appendExp("(load \"setup.scm\")")
                .appendExp("(load \"Test.scm\")")
                .appendExp("(define p (Cons 1 2))")
                .appendExp("(define pp (Cons 3 p))")
                .appendExp("(Car (Cdr pp))");

        auto ast = parseAllExpr(lex);
        auto res = ast->eval(s, ast);
        ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(res));
        auto numPtr = std::dynamic_pointer_cast<NumberAST>(res);
        ASSERT_EQ(1, numPtr->getValue());
    } catch (RuntimeError &e) {
        CLOG(DEBUG, "exception") << e.what();
        throw;
    }
}

