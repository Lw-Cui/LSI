#include <gtest/gtest.h>
#include <exception.h>
#include <parser.h>

using namespace lexers;
using namespace parser;
using namespace exception;

TEST(ExceptionParsingTest, DefinitionErrorTest) {
    lexers::Lexer lex{"(define)"};
    EXPECT_THROW(parseAllExpr(lex), UnsupportedSyntax);
}

TEST(ExceptionParsingTest, RawExprErrorTest) {
    lexers::Lexer lex;
    lex.appendExp("(if #t 0)");
    EXPECT_THROW(parseAllExpr(lex), NotAtomType);
}

TEST(ExceptionParsingTest, BracketTest) {
    lexers::Lexer lex;
    lex.appendExp("(define a 0");
    EXPECT_THROW(parseAllExpr(lex), MissBracket);

    lex.clear();
    lex.appendExp("()");
    EXPECT_THROW(parseAllExpr(lex), UnsupportedSyntax);
}

TEST(ExceptionParsingTest, LambdaDefinitionTest) {
    lexers::Lexer lex;
    lex.appendExp("(lambda () (+ x 1))");
    EXPECT_THROW(parseAllExpr(lex), UnexpectedType);
}