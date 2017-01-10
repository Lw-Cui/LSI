#include <memory>
#include <gtest/gtest.h>
#include <parser.h>

using namespace lexers;
using namespace parser;

TEST(BaseLibrariesParsingTest, BasicConsTest) {
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

TEST(BaseLibrariesParsingTest, AdvanceConsTest) {
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


TEST(BaseLibrariesParsingTest, MinusTest) {
    Scope s;
    lexers::Lexer lex("(load \"Base.scm\")");

    lex.appendExp("(- 5 7 8 9)");
    auto res = parseAllExpr(lex)->eval(s);
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(res));
    auto numPtr = std::dynamic_pointer_cast<NumberAST>(res);
    ASSERT_EQ(-19, numPtr->getValue());
}

TEST(BaseLibrariesParsingTest, NotTest) {
    Scope s;
    lexers::Lexer lex("(load \"Base.scm\")");

    lex.appendExp("(not (- 10 4 6))");
    auto res = parseAllExpr(lex)->eval(s);
    ASSERT_TRUE(std::dynamic_pointer_cast<BooleansFalseAST>(res));

    lex.appendExp("(not #f)");
    res = parseAllExpr(lex)->eval(s);
    ASSERT_TRUE(std::dynamic_pointer_cast<BooleansTrueAST>(res));
}

TEST(BaseLibrariesParsingTest, AndTest) {
    Scope s;
    lexers::Lexer lex("(load \"Base.scm\")");

    lex.appendExp("(and (+ 10 4 6) (- 10 1) (+ 5 6))");
    auto res = parseAllExpr(lex)->eval(s);
    ASSERT_TRUE(std::dynamic_pointer_cast<BooleansTrueAST>(res));

    lex.appendExp("(and (+ 10 4 6) #f (+ 5 7))");
    res = parseAllExpr(lex)->eval(s);
    ASSERT_TRUE(std::dynamic_pointer_cast<BooleansFalseAST>(res));
}

TEST(BaseLibrariesParsingTest, OrTest) {
    Scope s;
    lexers::Lexer lex("(load \"Base.scm\")");

    lex.appendExp("(or (+ 10 (#opposite 10)) (- 10 1) (- 1 10))");
    auto res = parseAllExpr(lex)->eval(s);
    ASSERT_TRUE(std::dynamic_pointer_cast<BooleansTrueAST>(res));

    lex.appendExp("(or #f (+ 0) 5)");
    res = parseAllExpr(lex)->eval(s);
    ASSERT_TRUE(std::dynamic_pointer_cast<BooleansTrueAST>(res));
}


TEST(BaseLibrariesParsingTest, GreaterTest) {
    Scope s;
    lexers::Lexer lex("(load \"Base.scm\")");

    lex.appendExp("(> (+ 10 (- 10)) 0)");
    auto res = parseAllExpr(lex)->eval(s);
    ASSERT_TRUE(std::dynamic_pointer_cast<BooleansFalseAST>(res));

    lex.appendExp("(> 13 (- 20 8) (+ 5 4))");
    res = parseAllExpr(lex)->eval(s);
    ASSERT_TRUE(std::dynamic_pointer_cast<BooleansTrueAST>(res));

    lex.appendExp("(> 12 (- 20 8) (+ 5 4))");
    res = parseAllExpr(lex)->eval(s);
    ASSERT_TRUE(std::dynamic_pointer_cast<BooleansFalseAST>(res));
}

TEST(BaseLibrariesParsingTest, EqualTest) {
    Scope s;
    lexers::Lexer lex("(load \"Base.scm\")");

    lex.appendExp("(= (+ 10 (- 10)) 0)");
    auto res = parseAllExpr(lex)->eval(s);
    ASSERT_TRUE(std::dynamic_pointer_cast<BooleansTrueAST>(res));

    lex.appendExp("(= 13 (- 20 7) (+ 5 8))");
    res = parseAllExpr(lex)->eval(s);
    ASSERT_TRUE(std::dynamic_pointer_cast<BooleansTrueAST>(res));

}

TEST(BaseLibrariesParsingTest, RemainderTest) {
    Scope s;
    lexers::Lexer lex("(load \"Base.scm\")");

    lex.appendExp("(remainder 5 2)");
    auto res = parseAllExpr(lex)->eval(s);
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(res));
    auto numPtr = std::dynamic_pointer_cast<NumberAST>(res);
    ASSERT_EQ(1, numPtr->getValue());
}

TEST(BaseLibrariesParsingTest, DivideTest) {
    Scope s;
    lexers::Lexer lex("(load \"Base.scm\")");

    lex.appendExp("(/ 60 6 2)");
    auto exprPtr = parseAllExpr(lex)->eval(s);
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(exprPtr));
    auto numPtr = std::dynamic_pointer_cast<NumberAST>(exprPtr);
    ASSERT_EQ(5, numPtr->getValue());
}

TEST(BaseLibrariesParsingTest, ReverseTest) {
    Scope s;
    lexers::Lexer lex("(load \"Base.scm\")");
    lex.appendExp("(reverse (list 1 2 3))");
    auto exprPtr = parseAllExpr(lex)->eval(s);
    ASSERT_STREQ("(3, (2, (1, '())))", exprPtr->display().c_str());
}

TEST(BaseLibrariesParsingTest, AppendTest) {
    Scope s;
    lexers::Lexer lex("(load \"Base.scm\")");
    lex.appendExp("(append (list 1 2 3) (list 4 5 6))");
    auto exprPtr = parseAllExpr(lex)->eval(s);
    ASSERT_STREQ("(1, (2, (3, (4, (5, (6, '()))))))", exprPtr->display().c_str());

    lex.appendExp("(append (list 6 5) (list 4 3) (list 2 1 0))");
    exprPtr = parseAllExpr(lex)->eval(s);
    ASSERT_STREQ("(6, (5, (4, (3, (2, (1, (0, '())))))))", exprPtr->display().c_str());
}

TEST(BaseLibrariesParsingTest, MapTest) {
    Scope s;
    lexers::Lexer lex("(load \"Base.scm\")");
    lex.appendExp("(map (list 1 2 3) (lambda (x) (+ x 1)))");
    auto exprPtr = parseAllExpr(lex)->eval(s);
    ASSERT_STREQ("(2, (3, (4, '())))", exprPtr->display().c_str());
}

TEST(BaseLibrariesParsingTest, ReduceTest) {
    Scope s;
    lexers::Lexer lex("(load \"Base.scm\")");

    lex.appendExp("(reduce (list 1 2 3) (lambda (res x) (+ res x)) 0)");
    auto exprPtr = parseAllExpr(lex)->eval(s);
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(exprPtr));
    auto numPtr = std::dynamic_pointer_cast<NumberAST>(exprPtr);
    ASSERT_EQ(6, numPtr->getValue());
}

TEST(BaseLibrariesParsingTest, sqrtTest) {
    Scope s;
    lexers::Lexer lex("(load \"Base.scm\")");

    lex.appendExp("(sqrt 4)");
    auto exprPtr = parseAllExpr(lex)->eval(s);
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(exprPtr));
    auto numPtr = std::dynamic_pointer_cast<NumberAST>(exprPtr);
    ASSERT_EQ(2, static_cast<int>(numPtr->getValue()));
}

TEST(BaseLibrariesParsingTest, YcombinatorTest) {
    Scope s;
    lexers::Lexer lex("(load \"Base.scm\")");

    lex.appendExp(""
                          "((Y (lambda (g)"
                          "     (lambda (x)"
                          "       (if (= x 0) 1"
                          "           (* x (g (- x 1))))))) 5)");
    auto exprPtr = parseAllExpr(lex)->eval(s);
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(exprPtr));
    auto numPtr = std::dynamic_pointer_cast<NumberAST>(exprPtr);
    ASSERT_EQ(120, numPtr->getValue());
}

