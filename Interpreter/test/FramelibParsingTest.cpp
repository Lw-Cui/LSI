#include <memory>
#include <gtest/gtest.h>
#include <parser.h>

using namespace lexers;
using namespace parser;

TEST(FrameLibParsingTest, VectorTest) {
    Scope s;
    lexers::Lexer lex;
    lex.appendExp("(load \"Base.scm\")").appendExp("(load \"Frame.scm\")");

    lex.appendExp("(define vect1 (make-vect 5 5))")
            .appendExp("(define vect2 (make-vect 2 3))");

    lex.appendExp("(xcor-vect vect1)");
    auto ptr = parseAllExpr(lex)->eval(s);
    ASSERT_STREQ("5", ptr->display().c_str());

    lex.appendExp("(add-vect vect1 vect2)");
    ptr = parseAllExpr(lex)->eval(s);
    ASSERT_STREQ("(7, 8)", ptr->display().c_str());

    lex.appendExp("(sub-vect (scale-vect 3 vect2) vect1)");
    ptr = parseAllExpr(lex)->eval(s);
    ASSERT_STREQ("(1, 4)", ptr->display().c_str());
}

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

TEST(FrameLibParsingTest, transformPainterTest) {
    Scope s;
    lexers::Lexer lex;
    lex.appendExp("(load \"Base.scm\")").appendExp("(load \"Frame.scm\")");

    lex.appendExp("(define (painter frame)"
                          "  (lambda (vect)"
                          "    ((frame-coord-map frame) vect)))");
    lex.appendExp("(((flip-vert painter) default) (cons 100 100))");
    auto ptr = parseAllExpr(lex)->eval(s);
    ASSERT_STREQ("(100, 900)", ptr->display().c_str());

    lex.appendExp("(((shrink-to-upper-right painter) default) (cons 100 100))");
    ptr = parseAllExpr(lex)->eval(s);
    ASSERT_STREQ("(550, 550)", ptr->display().c_str());

    lex.appendExp("(((shrink-to-upper-left painter) default) (cons 100 100))");
    ptr = parseAllExpr(lex)->eval(s);
    ASSERT_STREQ("(50, 550)", ptr->display().c_str());

    lex.appendExp("(((rotate90 painter) default) (cons -100 100))");
    ptr = parseAllExpr(lex)->eval(s);
    ASSERT_STREQ("(900, -100)", ptr->display().c_str());
}

TEST(FrameLibParsingTest, MultpletransformPainterTest) {
    Scope s;
    lexers::Lexer lex;
    lex.appendExp("(load \"Base.scm\")").appendExp("(load \"Frame.scm\")");

    lex.appendExp("(define (painter frame)"
                          "  (lambda (vect)"
                          "    ((frame-coord-map frame) vect)))");
    lex.appendExp("(((rotate90 (flip-vert painter)) default) (cons 100 100))");
    auto ptr = parseAllExpr(lex)->eval(s);
    ASSERT_STREQ("(100, 100)", ptr->display().c_str());

    lex.appendExp("(((shrink-to-upper-left(rotate90 (flip-vert painter))) default) (cons 100 100))");
    ptr = parseAllExpr(lex)->eval(s);
    ASSERT_STREQ("(50, 550)", ptr->display().c_str());
}
