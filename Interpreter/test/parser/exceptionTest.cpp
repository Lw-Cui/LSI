#include <gtest/gtest.h>
#include <exception.h>
#include <parser.h>

using namespace lexers;
using namespace parser;
using namespace exception;

TEST(ExceptionParsingTest, DefinitionErrorTest) {
    lexers::Lexer lex{"(define)"};
    EXPECT_THROW(parseAllExpr(lex), exception::DefinitionError);
}

