#include <memory>
#include <gtest/gtest.h>
#include <parser.h>
#include <exception.h>
#include <testMacro.h>
#include <visitor.h>

using namespace lexers;
using namespace parser;
using namespace exception;
using std::make_shared;

TEST(FrameLibParsingTest, VectorTest) {
    BEG_TRY
        CREATE_CONTEXT();
        lex.appendExp("(load \"setup.scm\")");
        REPL_COND("(define vect1 (make-vect 5 5))"
                          "(define vect2 (make-vect 2 3))"
                          "(xcor-vect vect1)", true);
        ASSERT_STREQ("5", disp.to_string().c_str());

        REPL_COND("(add-vect vect1 vect2)", true);
        ASSERT_STREQ("(7, 8)", disp.to_string().c_str());

        REPL_COND("(sub-vect (scale-vect 3 vect2) vect1)", true);
        ASSERT_STREQ("(1, 4)", disp.to_string().c_str());
    END_TRY
}

TEST(FrameLibParsingTest, FrameBasicTest) {
    BEG_TRY
        CREATE_CONTEXT()
        lex.appendExp("(load \"setup.scm\")");
        REPL_COND("(define frame (make-frame (cons 50 40) (cons 5 4) (cons 4 5)))"
                          "(origin-frame frame)", true);
        ASSERT_STREQ("(50, 40)", disp.to_string().c_str());

        REPL_COND("(edgeX-frame frame)", true);
        ASSERT_STREQ("(5, 4)", disp.to_string().c_str());

        REPL_COND("(edgeY-frame frame)", true);
        ASSERT_STREQ("(4, 5)", disp.to_string().c_str());
    END_TRY
}

TEST(FrameLibParsingTest, CoordinateMapTest) {
    BEG_TRY
        CREATE_CONTEXT()
        REPL_COND("(load \"setup.scm\")"
                          "(define frame (make-frame (cons 0.5 0.5) (cons -0.5 -0.5) (cons 0.5 -0.5)))"
                          "(define coord-map (frame-coord-map frame))"
                          "(coord-map (cons 500 50))", true);
        ASSERT_STREQ("(-224.5, -274.5)", disp.to_string().c_str());

        REPL_COND("(define frame2 (make-frame (cons 1 1) (cons 0 -1) (cons -1 0)))"
                          "((frame-coord-map frame2) (cons 100 300))", true);
        ASSERT_STREQ("(-299, -99)", disp.to_string().c_str());
    END_TRY
}

TEST(FrameLibParsingTest, transformPainterTest) {
    BEG_TRY
        CREATE_CONTEXT();
        REPL_COND("(load \"setup.scm\")"
                          "(define (painter frame)"
                          "  (lambda (vect)"
                          "    ((frame-coord-map frame) vect)))"
                          "(((flip-vert painter) default) (cons 100 100))", true);
        ASSERT_STREQ("(100, -99)", disp.to_string().c_str());

        REPL_COND("(((shrink-to-upper-right painter) default) (cons 100 100))", true);
        ASSERT_STREQ("(50.5, 50.5)", disp.to_string().c_str());

        REPL_COND("(((shrink-to-upper-left painter) default) (cons 100 100))", true);
        ASSERT_STREQ("(50, 50.5)", disp.to_string().c_str());

        REPL_COND("(((rotate90 painter) default) (cons -100 100))", true);
        ASSERT_STREQ("(-99, -100)", disp.to_string().c_str());
    END_TRY
}

TEST(FrameLibParsingTest, MultpletransformPainterTest) {
    BEG_TRY
        CREATE_CONTEXT();
        REPL_COND("(load \"setup.scm\")"
                          "(define (painter frame)"
                          "  (lambda (vect)"
                          "    ((frame-coord-map frame) vect)))"
                          "(((rotate90 (flip-vert painter)) default) (cons 100 100))", true);
        ASSERT_STREQ("(100, 100)", disp.to_string().c_str());

        REPL_COND("(((shrink-to-upper-left(rotate90 (flip-vert painter))) default) (cons 100 100))", true);
        ASSERT_STREQ("(50, 50.5)", disp.to_string().c_str());
    END_TRY
}
