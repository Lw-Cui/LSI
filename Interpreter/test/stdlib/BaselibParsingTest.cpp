#include <memory>
#include <gtest/gtest.h>
#include <parser.h>
#include <exception.h>
#include <testMacro.h>

using namespace lexers;
using namespace parser;
using namespace exception;
using std::make_shared;

TEST(BaseLibrariesParsingTest, MinusTest) {
    BEG_TRY
        CREATE_CONTEXT();
        lex.appendExp("(load \"setup.scm\")");
        REPL_COND("(- 5 7 8 9)", TO_NUM_PTR(res));
        ASSERT_EQ(-19, numPtr->getValue());
    END_TRY
}

TEST(BaseLibrariesParsingTest, NotTest) {
    BEG_TRY
        CREATE_CONTEXT();
        lex.appendExp("(load \"setup.scm\")");
        REPL_COND("(not (- 10 4 6))", TO_FALSE_PTR(res));
        REPL_COND("(not #f)", TO_TRUE_PTR(res));
    END_TRY
}

TEST(BaseLibrariesParsingTest, AndTest) {
    BEG_TRY
        CREATE_CONTEXT();
        lex.appendExp("(load \"setup.scm\")");
        REPL_COND("(and (+ 10 4 6) (- 10 1) (+ 5 6))", TO_TRUE_PTR(res));
        REPL_COND("(and (+ 10 4 6) #f (+ 5 7))", TO_FALSE_PTR(res));
    END_TRY
}

TEST(BaseLibrariesParsingTest, OrTest) {
    BEG_TRY
        CREATE_CONTEXT();
        lex.appendExp("(load \"setup.scm\")");
        REPL_COND("(or (+ 10 (#opposite 10)) (- 10 1) (- 1 10))", TO_TRUE_PTR(res));
        REPL_COND("(or #f (+ 0) 5)", TO_TRUE_PTR(res));
    END_TRY
}


TEST(BaseLibrariesParsingTest, GreaterTest) {
    BEG_TRY
        CREATE_CONTEXT();
        lex.appendExp("(load \"setup.scm\")");
        REPL_COND("(> (+ 10 (- 10)) 0)", TO_FALSE_PTR(res));
        REPL_COND("(> 13 (- 20 8) (+ 5 4))", TO_TRUE_PTR(res));
        REPL_COND("(> 12 (- 20 8) (+ 5 4))", TO_FALSE_PTR(res));
    END_TRY
}

TEST(BaseLibrariesParsingTest, EqualTest) {
    BEG_TRY
        CREATE_CONTEXT();
        lex.appendExp("(load \"setup.scm\")");
        REPL_COND("(= (+ 10 (- 10)) 0)", TO_TRUE_PTR(res));
        REPL_COND("(= 13 (- 20 7) (+ 5 8))", TO_TRUE_PTR(res));
    END_TRY
}

TEST(BaseLibrariesParsingTest, RemainderTest) {
    BEG_TRY
        CREATE_CONTEXT();
        lex.appendExp("(load \"setup.scm\")");
        REPL_COND("(remainder 5 2)", TO_NUM_PTR(res));
        ASSERT_EQ(1, numPtr->getValue());
    END_TRY
}

TEST(BaseLibrariesParsingTest, DivideTest) {
    BEG_TRY
        CREATE_CONTEXT();
        lex.appendExp("(load \"setup.scm\")");
        REPL_COND("(/ 60 6 2)", TO_NUM_PTR(res));
        ASSERT_EQ(5, numPtr->getValue());
    END_TRY
}

TEST(BaseLibrariesParsingTest, ReverseTest) {
    BEG_TRY
        CREATE_CONTEXT();
        lex.appendExp("(load \"setup.scm\")");
        REPL_COND("(reverse (list 1 2 3))", true);
        ASSERT_STREQ("(3, (2, (1, '())))", disp.to_string().c_str());
    END_TRY
}

TEST(BaseLibrariesParsingTest, AppendTest) {
    BEG_TRY
        CREATE_CONTEXT();
        lex.appendExp("(load \"setup.scm\")");
        REPL_COND("(append (list 1 2 3) (list 4 5 6))", true);
        ASSERT_STREQ("(1, (2, (3, (4, (5, (6, '()))))))", disp.to_string().c_str());

        REPL_COND("(append (list 6 5) (list 4 3) (list 2 1 0))", true);
        ASSERT_STREQ("(6, (5, (4, (3, (2, (1, (0, '())))))))", disp.to_string().c_str());
    END_TRY
}

TEST(BaseLibrariesParsingTest, MapTest) {
    BEG_TRY
        CREATE_CONTEXT();
        lex.appendExp("(load \"setup.scm\")");
        REPL_COND("(map (list 1 2 3) (lambda (x) (+ x 1)))", true);
        ASSERT_STREQ("(2, (3, (4, '())))", disp.to_string().c_str());
    END_TRY
}

TEST(BaseLibrariesParsingTest, ReduceTest) {
    BEG_TRY
        CREATE_CONTEXT();
        lex.appendExp("(load \"setup.scm\")");
        REPL_COND("(reduce (list 1 2 3) (lambda (res x) (+ res x)) 0)", TO_NUM_PTR(res));
        ASSERT_EQ(6, numPtr->getValue());
    END_TRY
}

TEST(BaseLibrariesParsingTest, sqrtTest) {
    BEG_TRY
        CREATE_CONTEXT();
        lex.appendExp("(load \"setup.scm\")");
        REPL_COND("(sqrt 4)", TO_NUM_PTR(res));
        ASSERT_EQ(2, static_cast<int>(numPtr->getValue()));
    END_TRY
}

TEST(BaseLibrariesParsingTest, YcombinatorTest) {
    BEG_TRY
        CREATE_CONTEXT();
        lex.appendExp("(load \"setup.scm\")");
        REPL_COND(""
                          "((Y (lambda (g)"
                          "     (lambda (x)"
                          "       (if (= x 0) 1"
                          "           (* x (g (- x 1))))))) 5)", TO_NUM_PTR(res));
        ASSERT_EQ(120, numPtr->getValue());
    END_TRY
}

TEST(BaseLibrariesParsingTest, lengthTest) {
    BEG_TRY
        CREATE_CONTEXT();
        lex.appendExp("(load \"setup.scm\")");
        REPL_COND("(length (list 5 7 9 0))", TO_NUM_PTR(res));
        ASSERT_EQ(4, numPtr->getValue());
    END_TRY
}
