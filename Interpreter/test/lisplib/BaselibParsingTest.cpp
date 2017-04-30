#include <memory>
#include <gtest/gtest.h>
#include <parser.h>
#include <exception.h>

using namespace lexers;
using namespace parser;
using namespace exception;
using std::make_shared;

TEST(BaseLibrariesParsingTest, MinusTest) {
    try {
        auto s = make_shared<Scope>();
        lexers::Lexer lex("(load \"setup.scm\")");

        lex.appendExp("(- 5 7 8 9)");
        auto res = parseAllExpr(lex)->eval(s);
        ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(res));
        auto numPtr = std::dynamic_pointer_cast<NumberAST>(res);
        ASSERT_EQ(-19, numPtr->getValue());
    } catch (RuntimeError &e) {
        CLOG(DEBUG, "exception") << e.what();
        throw;
    }
}

TEST(BaseLibrariesParsingTest, NotTest) {
    try {
        auto s = make_shared<Scope>();
        lexers::Lexer lex("(load \"setup.scm\")");

        lex.appendExp("(not (- 10 4 6))");
        auto res = parseAllExpr(lex)->eval(s);
        ASSERT_TRUE(std::dynamic_pointer_cast<BooleansFalseAST>(res));

        lex.appendExp("(not #f)");
        res = parseAllExpr(lex)->eval(s);
        ASSERT_TRUE(std::dynamic_pointer_cast<BooleansTrueAST>(res));
    } catch (RuntimeError &e) {
        CLOG(DEBUG, "exception") << e.what();
        throw;
    }
}

TEST(BaseLibrariesParsingTest, AndTest) {
    try {
        auto s = make_shared<Scope>();
        lexers::Lexer lex("(load \"setup.scm\")");

        lex.appendExp("(and (+ 10 4 6) (- 10 1) (+ 5 6))");
        auto res = parseAllExpr(lex)->eval(s);
        ASSERT_TRUE(std::dynamic_pointer_cast<BooleansTrueAST>(res));

        lex.appendExp("(and (+ 10 4 6) #f (+ 5 7))");
        res = parseAllExpr(lex)->eval(s);
        ASSERT_TRUE(std::dynamic_pointer_cast<BooleansFalseAST>(res));
    } catch (RuntimeError &e) {
        CLOG(DEBUG, "exception") << e.what();
        throw;
    }
}

TEST(BaseLibrariesParsingTest, OrTest) {
    auto s = make_shared<Scope>();
    lexers::Lexer lex("(load \"setup.scm\")");

    lex.appendExp("(or (+ 10 (#opposite 10)) (- 10 1) (- 1 10))");
    try {
        auto res = parseAllExpr(lex)->eval(s);
        ASSERT_TRUE(std::dynamic_pointer_cast<BooleansTrueAST>(res));

        lex.appendExp("(or #f (+ 0) 5)");
        res = parseAllExpr(lex)->eval(s);
        ASSERT_TRUE(std::dynamic_pointer_cast<BooleansTrueAST>(res));
    } catch (RuntimeError &e) {
        CLOG(DEBUG, "exception") << e.what();
        throw;
    }
}


TEST(BaseLibrariesParsingTest, GreaterTest) {
    try {
        auto s = make_shared<Scope>();
        lexers::Lexer lex("(load \"setup.scm\")");

        lex.appendExp("(> (+ 10 (- 10)) 0)");
        auto res = parseAllExpr(lex)->eval(s);
        ASSERT_TRUE(std::dynamic_pointer_cast<BooleansFalseAST>(res));

        lex.appendExp("(> 13 (- 20 8) (+ 5 4))");
        res = parseAllExpr(lex)->eval(s);
        ASSERT_TRUE(std::dynamic_pointer_cast<BooleansTrueAST>(res));

        lex.appendExp("(> 12 (- 20 8) (+ 5 4))");
        res = parseAllExpr(lex)->eval(s);
        ASSERT_TRUE(std::dynamic_pointer_cast<BooleansFalseAST>(res));
    } catch (RuntimeError &e) {
        CLOG(DEBUG, "exception") << e.what();
        throw;
    }
}

TEST(BaseLibrariesParsingTest, EqualTest) {
    try {
        auto s = make_shared<Scope>();
        lexers::Lexer lex("(load \"setup.scm\")");

        lex.appendExp("(= (+ 10 (- 10)) 0)");
        auto res = parseAllExpr(lex)->eval(s);
        ASSERT_TRUE(std::dynamic_pointer_cast<BooleansTrueAST>(res));

        lex.appendExp("(= 13 (- 20 7) (+ 5 8))");
        res = parseAllExpr(lex)->eval(s);
        ASSERT_TRUE(std::dynamic_pointer_cast<BooleansTrueAST>(res));

    } catch (RuntimeError &e) {
        CLOG(DEBUG, "exception") << e.what();
        throw;
    }
}

TEST(BaseLibrariesParsingTest, RemainderTest) {
    try {
        auto s = make_shared<Scope>();
        lexers::Lexer lex("(load \"setup.scm\")");

        lex.appendExp("(remainder 5 2)");
        auto res = parseAllExpr(lex)->eval(s);
        ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(res));
        auto numPtr = std::dynamic_pointer_cast<NumberAST>(res);
        ASSERT_EQ(1, numPtr->getValue());
    } catch (RuntimeError &e) {
        CLOG(DEBUG, "exception") << e.what();
        throw;
    }
}

TEST(BaseLibrariesParsingTest, DivideTest) {
    auto s = make_shared<Scope>();
    lexers::Lexer lex("(load \"setup.scm\")");

    try {
        lex.appendExp("(/ 60 6 2)");
        auto exprPtr = parseAllExpr(lex)->eval(s);
        ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(exprPtr));
        auto numPtr = std::dynamic_pointer_cast<NumberAST>(exprPtr);
        ASSERT_EQ(5, numPtr->getValue());
    } catch (RuntimeError &e) {
        CLOG(DEBUG, "exception") << e.what();
        throw;
    }
}

TEST(BaseLibrariesParsingTest, ReverseTest) {
    auto s = make_shared<Scope>();
    try {
        lexers::Lexer lex("(load \"setup.scm\")");
        lex.appendExp("(reverse (list 1 2 3))");
        auto exprPtr = parseAllExpr(lex)->eval(s);
        ASSERT_STREQ("(3, (2, (1, '())))", exprPtr->display().c_str());
    } catch (RuntimeError &e) {
        CLOG(DEBUG, "exception") << e.what();
        throw;
    }
}

TEST(BaseLibrariesParsingTest, AppendTest) {
    auto s = make_shared<Scope>();
    lexers::Lexer lex("(load \"setup.scm\")");
    try {
        lex.appendExp("(append (list 1 2 3) (list 4 5 6))");
        auto exprPtr = parseAllExpr(lex)->eval(s);
        ASSERT_STREQ("(1, (2, (3, (4, (5, (6, '()))))))", exprPtr->display().c_str());

        lex.appendExp("(append (list 6 5) (list 4 3) (list 2 1 0))");
        exprPtr = parseAllExpr(lex)->eval(s);
        ASSERT_STREQ("(6, (5, (4, (3, (2, (1, (0, '())))))))", exprPtr->display().c_str());
    } catch (RuntimeError &e) {
        CLOG(DEBUG, "exception") << e.what();
        throw;
    }
}

TEST(BaseLibrariesParsingTest, MapTest) {
    auto s = make_shared<Scope>();
    try {
        lexers::Lexer lex("(load \"setup.scm\")");
        lex.appendExp("(map (list 1 2 3) (lambda (x) (+ x 1)))");
        auto exprPtr = parseAllExpr(lex)->eval(s);
        ASSERT_STREQ("(2, (3, (4, '())))", exprPtr->display().c_str());
    } catch (RuntimeError &e) {
        CLOG(DEBUG, "exception") << e.what();
        throw;
    }
}

TEST(BaseLibrariesParsingTest, ReduceTest) {
    auto s = make_shared<Scope>();
    lexers::Lexer lex("(load \"setup.scm\")");

    lex.appendExp("(reduce (list 1 2 3) (lambda (res x) (+ res x)) 0)");
    try {
        auto exprPtr = parseAllExpr(lex)->eval(s);
        ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(exprPtr));
        auto numPtr = std::dynamic_pointer_cast<NumberAST>(exprPtr);
        ASSERT_EQ(6, numPtr->getValue());
    } catch (RuntimeError &e) {
        CLOG(DEBUG, "exception") << e.what();
        throw;
    }
}

TEST(BaseLibrariesParsingTest, sqrtTest) {
    auto s = make_shared<Scope>();
    lexers::Lexer lex("(load \"setup.scm\")");

    try {
        lex.appendExp("(sqrt 4)");
        auto exprPtr = parseAllExpr(lex)->eval(s);
        ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(exprPtr));
        auto numPtr = std::dynamic_pointer_cast<NumberAST>(exprPtr);
        ASSERT_EQ(2, static_cast<int>(numPtr->getValue()));
    } catch (RuntimeError &e) {
        CLOG(DEBUG, "exception") << e.what();
        throw;
    }
}

TEST(BaseLibrariesParsingTest, YcombinatorTest) {
    auto s = make_shared<Scope>();
    lexers::Lexer lex("(load \"setup.scm\")");

    lex.appendExp(""
                          "((Y (lambda (g)"
                          "     (lambda (x)"
                          "       (if (= x 0) 1"
                          "           (* x (g (- x 1))))))) 5)");
    try {
        auto exprPtr = parseAllExpr(lex)->eval(s);
        ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(exprPtr));
        auto numPtr = std::dynamic_pointer_cast<NumberAST>(exprPtr);
        ASSERT_EQ(120, numPtr->getValue());
    } catch (RuntimeError &e) {
        CLOG(DEBUG, "exception") << e.what();
        throw;
    }
}

TEST(BaseLibrariesParsingTest, lengthTest) {
    try {
        auto s = make_shared<Scope>();
        lexers::Lexer lex("(load \"setup.scm\")");

        lex.appendExp("(length (list 5 7 9 0))");
        auto exprPtr = parseAllExpr(lex)->eval(s);
        ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(exprPtr));
        auto numPtr = std::dynamic_pointer_cast<NumberAST>(exprPtr);
        ASSERT_EQ(4, numPtr->getValue());
    } catch (RuntimeError &e) {
        CLOG(DEBUG, "exception") << e.what();
        throw;
    }
}
