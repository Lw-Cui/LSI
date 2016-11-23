#include <memory>
#include <gtest/gtest.h>
#include <parser.h>

using namespace lexers;
using namespace parser;

TEST(LibrariesParsingTest, BasicConsTest) {
    Scope s;
    lexers::Lexer lex;
    lex.appendExp("(load \"Base.scm\")").appendExp("(load \"Test.scm\")");
    parseAllExpr(lex)->eval(s);
    ASSERT_TRUE(s.count("Cons"));
    ASSERT_TRUE(s.count("Car"));
    ASSERT_TRUE(s.count("Cdr"));

    lex.appendExp("(define p (Cons 1 2))").appendExp("(Cdr p)");
    auto res = parseAllExpr(lex)->eval(s);
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(res));
    auto numPtr = std::dynamic_pointer_cast<NumberAST>(res);
    ASSERT_EQ(2, numPtr->getValue());

    lex.appendExp("(Car p)");
    res = parseAllExpr(lex)->eval(s);
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(res));
    numPtr = std::dynamic_pointer_cast<NumberAST>(res);
    ASSERT_EQ(1, numPtr->getValue());
}

TEST(LibrariesParsingTest, AdvanceConsTest) {
    Scope s;
    lexers::Lexer lex;

    lex.appendExp("(load \"Base.scm\")")
            .appendExp("(load \"Test.scm\")")
            .appendExp("(define p (Cons 1 2))")
            .appendExp("(define pp (Cons 3 p))")
            .appendExp("(Car (Cdr pp))");

    auto res = parseAllExpr(lex)->eval(s);
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(res));
    auto numPtr = std::dynamic_pointer_cast<NumberAST>(res);
    ASSERT_EQ(1, numPtr->getValue());
}


TEST(LibrariesParsingTest, MinusTest) {
    Scope s;
    lexers::Lexer lex("(load \"Base.scm\")");

    lex.appendExp("(- 5 7 8 9)");
    auto res = parseAllExpr(lex)->eval(s);
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(res));
    auto numPtr = std::dynamic_pointer_cast<NumberAST>(res);
    ASSERT_EQ(-19, numPtr->getValue());
}

TEST(LibrariesParsingTest, NotTest) {
    Scope s;
    lexers::Lexer lex("(load \"Base.scm\")");

    lex.appendExp("(not (- 10 4 6))");
    auto res = parseAllExpr(lex)->eval(s);
    ASSERT_TRUE(std::dynamic_pointer_cast<BooleansFalseAST>(res));

    lex.appendExp("(not #f)");
    res = parseAllExpr(lex)->eval(s);
    ASSERT_TRUE(std::dynamic_pointer_cast<BooleansTrueAST>(res));
}

TEST(LibrariesParsingTest, AndTest) {
    Scope s;
    lexers::Lexer lex("(load \"Base.scm\")");

    lex.appendExp("(and (+ 10 4 6) (- 10 1) (+ 5 6))");
    auto res = parseAllExpr(lex)->eval(s);
    ASSERT_TRUE(std::dynamic_pointer_cast<BooleansTrueAST>(res));

    lex.appendExp("(and (+ 10 4 6) #f (+ 5 7))");
    res = parseAllExpr(lex)->eval(s);
    ASSERT_TRUE(std::dynamic_pointer_cast<BooleansFalseAST>(res));
}

TEST(LibrariesParsingTest, OrTest) {
    Scope s;
    lexers::Lexer lex("(load \"Base.scm\")");

    lex.appendExp("(or (+ 10 (#opposite 10)) (- 10 1) (- 1 10))");
    auto res = parseAllExpr(lex)->eval(s);
    ASSERT_TRUE(std::dynamic_pointer_cast<BooleansTrueAST>(res));

    lex.appendExp("(or #f (+ 0) 5)");
    res = parseAllExpr(lex)->eval(s);
    ASSERT_TRUE(std::dynamic_pointer_cast<BooleansTrueAST>(res));
}

TEST(LibrariesParsingTest, EqualTest) {
    Scope s;
    lexers::Lexer lex("(load \"Base.scm\")");

    lex.appendExp("(= (+ 10 (- 10)) 0)");
    auto res = parseAllExpr(lex)->eval(s);
    ASSERT_TRUE(std::dynamic_pointer_cast<BooleansTrueAST>(res));

    lex.appendExp("(= 13 (- 20 7) (+ 5 8))");
    res = parseAllExpr(lex)->eval(s);
    ASSERT_TRUE(std::dynamic_pointer_cast<BooleansTrueAST>(res));

}

TEST(LibrariesParsingTest, RemainderTest) {
    Scope s;
    lexers::Lexer lex("(load \"Base.scm\")");

    lex.appendExp("(remainder 5 2)");
    auto res = parseAllExpr(lex)->eval(s);
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(res));
    auto numPtr = std::dynamic_pointer_cast<NumberAST>(res);
    ASSERT_EQ(1, numPtr->getValue());
}

TEST(LibrariesParsingTest, DivideTest) {
    Scope s;
    lexers::Lexer lex("(load \"Base.scm\")");

    lex.appendExp("(/ 60 6 2)");
    auto exprPtr = parseAllExpr(lex)->eval(s);
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(exprPtr));
    auto numPtr = std::dynamic_pointer_cast<NumberAST>(exprPtr);
    ASSERT_EQ(5, numPtr->getValue());
}

