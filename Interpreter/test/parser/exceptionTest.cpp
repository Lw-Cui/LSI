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
    EXPECT_THROW(parseAllExpr(lex), UnsupportedSyntax);

    lex.clear();
    lex.appendExp("(lambda (x (+ x 1))");
    EXPECT_THROW(parseAllExpr(lex), MissBracket);
}

TEST(ExceptionParsingTest, FunctionDefinitionTest) {
    lexers::Lexer lex;
    lex.appendExp("(define () (+ x 1))");
    EXPECT_THROW(parseAllExpr(lex), UnsupportedSyntax);

    lex.clear();
    lex.appendExp("(define (x (+ x 1))");
    EXPECT_THROW(parseAllExpr(lex), MissBracket);
}

TEST(ExceptionParsingTest, BuiltinLessThanASTTest) {
    lexers::Lexer lex;
    auto ss = std::make_shared<Scope>();
    lex.appendExp("(< 5 +)");
    EXPECT_THROW(parseAllExpr(lex)->eval(ss), NotNumber);

    lex.clear();
    ss->clear();
    lex.appendExp("x");
    EXPECT_THROW(parseAllExpr(lex)->eval(ss), UnboundIdentifier);
}

TEST(ExceptionParsingTest, BuiltinConsTest) {
    lexers::Lexer lex;
    auto ss = std::make_shared<Scope>();
    lex.appendExp("(car 5)");
    EXPECT_THROW(parseAllExpr(lex)->eval(ss), NotPair);

    lex.clear();
    ss->clear();
    lex.appendExp("(cons 5)");
    EXPECT_THROW(parseAllExpr(lex)->eval(ss), NotPair);
}
