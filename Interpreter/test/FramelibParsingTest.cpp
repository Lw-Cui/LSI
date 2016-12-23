#include <memory>
#include <gtest/gtest.h>
#include <parser.h>

using namespace lexers;
using namespace parser;

TEST(FrameLibParsingTest, FrameBasicTest) {
    Scope s;
    lexers::Lexer lex;
    lex.appendExp("(load \"Base.scm\")").appendExp("(load \"Frame.scm\")");
    lex.appendExp("(define frame (make-frame (cons 50 40) (cons 5 4) (cons 4 5)))");

    lex.appendExp("(origin-frame frame)");
    auto ptr = parseAllExpr(lex)->eval(s);
    ASSERT_STREQ("(50, 40)", ptr->display().c_str());

    lex.appendExp("(edge1-frame frame)");
    ptr = parseAllExpr(lex)->eval(s);
    ASSERT_STREQ("(5, 4)", ptr->display().c_str());

    lex.appendExp("(edge2-frame frame)");
    ptr = parseAllExpr(lex)->eval(s);
    ASSERT_STREQ("(4, 5)", ptr->display().c_str());
}

TEST(FrameLibParsingTest, CoordinateMapTest) {
    Scope s;
    lexers::Lexer lex;
    lex.appendExp("(load \"Base.scm\")").appendExp("(load \"Frame.scm\")");

    lex.appendExp("(define frame (make-frame (cons 500 500) (cons -500 -500) (cons 500 -500)))")
            .appendExp("(define coord-map (frame-coord-map frame))")
            .appendExp("(coord-map (cons 500 500))");
    auto ptr = parseAllExpr(lex)->eval(s);
    ASSERT_STREQ("(500, 0)", ptr->display().c_str());

    lex.appendExp("(define frame2 (make-frame (cons 1000 1000) (cons 0 -1000) (cons -1000 0)))")
            .appendExp("((frame-coord-map frame2) (cons 100 300))");
    ptr = parseAllExpr(lex)->eval(s);
    ASSERT_STREQ("(700, 900)", ptr->display().c_str());
}
