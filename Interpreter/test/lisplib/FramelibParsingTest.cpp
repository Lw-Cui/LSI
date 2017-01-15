#include <memory>
#include <gtest/gtest.h>
#include <parser.h>
#include <exception.h>

using namespace lexers;
using namespace parser;
using namespace exception;

TEST(FrameLibParsingTest, VectorTest) {
    Scope s;
    lexers::Lexer lex;
    lex.appendExp("(load \"setup.scm\")");

    lex.appendExp("(define vect1 (make-vect 5 5))")
            .appendExp("(define vect2 (make-vect 2 3))");

    lex.appendExp("(xcor-vect vect1)");
    try {
        auto ptr = parseAllExpr(lex)->eval(s);
        ASSERT_STREQ("5", ptr->display().c_str());

        lex.appendExp("(add-vect vect1 vect2)");
        ptr = parseAllExpr(lex)->eval(s);
        ASSERT_STREQ("(7, 8)", ptr->display().c_str());

        lex.appendExp("(sub-vect (scale-vect 3 vect2) vect1)");
        ptr = parseAllExpr(lex)->eval(s);
        ASSERT_STREQ("(1, 4)", ptr->display().c_str());
    } catch (RuntimeError &e) {
        CLOG(DEBUG, "exception") << e.what();
        throw;
    }
}

TEST(FrameLibParsingTest, FrameBasicTest) {
    Scope s;
    lexers::Lexer lex;
    lex.appendExp("(load \"setup.scm\")");
    lex.appendExp("(define frame (make-frame (cons 50 40) (cons 5 4) (cons 4 5)))");

    lex.appendExp("(origin-frame frame)");
    try {
        auto ptr = parseAllExpr(lex)->eval(s);
        ASSERT_STREQ("(50, 40)", ptr->display().c_str());

        lex.appendExp("(edgeX-frame frame)");
        ptr = parseAllExpr(lex)->eval(s);
        ASSERT_STREQ("(5, 4)", ptr->display().c_str());

        lex.appendExp("(edgeY-frame frame)");
        ptr = parseAllExpr(lex)->eval(s);
        ASSERT_STREQ("(4, 5)", ptr->display().c_str());
    } catch (RuntimeError &e) {
        CLOG(DEBUG, "exception") << e.what();
        throw;
    }
}

TEST(FrameLibParsingTest, CoordinateMapTest) {
    try {
        Scope s;
        lexers::Lexer lex;
        lex.appendExp("(load \"setup.scm\")");

        lex.appendExp("(define frame (make-frame (cons 0.5 0.5) (cons -0.5 -0.5) (cons 0.5 -0.5)))")
                .appendExp("(define coord-map (frame-coord-map (ratio-to-reality frame)))")
                .appendExp("(coord-map (cons 500 50))");
        auto ptr = parseAllExpr(lex)->eval(s);
        ASSERT_STREQ("(275, 225)", ptr->display().c_str());

        lex.appendExp("(define frame2 (make-frame (cons 1 1) (cons 0 -1) (cons -1 0)))")
                .appendExp("((frame-coord-map (ratio-to-reality frame2)) (cons 100 300))");
        ptr = parseAllExpr(lex)->eval(s);
        ASSERT_STREQ("(700, 900)", ptr->display().c_str());
    } catch (RuntimeError &e) {
        CLOG(DEBUG, "exception") << e.what();
        throw;
    }
}

TEST(FrameLibParsingTest, transformPainterTest) {
    try {
        Scope s;
        lexers::Lexer lex;
        lex.appendExp("(load \"setup.scm\")");

        lex.appendExp("(define (painter frame)"
                              "  (lambda (vect)"
                              "    ((frame-coord-map (ratio-to-reality frame)) vect)))");
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
    } catch (RuntimeError &e) {
        CLOG(DEBUG, "exception") << e.what();
        throw;
    }
}

TEST(FrameLibParsingTest, MultpletransformPainterTest) {
    try {
        Scope s;
        lexers::Lexer lex;
        lex.appendExp("(load \"setup.scm\")");

        lex.appendExp("(define (painter frame)"
                              "  (lambda (vect)"
                              "    ((frame-coord-map (ratio-to-reality frame)) vect)))");
        lex.appendExp("(((rotate90 (flip-vert painter)) default) (cons 100 100))");
        auto ptr = parseAllExpr(lex)->eval(s);
        ASSERT_STREQ("(100, 100)", ptr->display().c_str());

        lex.appendExp("(((shrink-to-upper-left(rotate90 (flip-vert painter))) default) (cons 100 100))");
        ptr = parseAllExpr(lex)->eval(s);
        ASSERT_STREQ("(50, 550)", ptr->display().c_str());
    } catch (RuntimeError &e) {
        CLOG(DEBUG, "exception") << e.what();
        throw;
    }
}
