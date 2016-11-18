#include <memory>
#include <gtest/gtest.h>
#include <parser.h>

using namespace lexers;
using namespace parser;

TEST(LibrariesParsingTest, ConsTest) {
    Scope s;
    lexers::Lexer lex("(load \"Base.scm\")");
    parseExpr(lex)->eval(s);
    ASSERT_TRUE(s.count("cons"));
    ASSERT_TRUE(s.count("car"));
    ASSERT_TRUE(s.count("cdr"));

    lex.appendExp("(define p (cons 1 2))").appendExp("(cdr p)");
    auto res = parseAllExpr(lex)->eval(s);
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(res));
    auto numPtr = std::dynamic_pointer_cast<NumberAST>(res);
    ASSERT_EQ(1, numPtr->getValue());
}
