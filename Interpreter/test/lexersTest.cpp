#include <gtest/gtest.h>
#include <parser.h>

using namespace lexers;

TEST(LexersTest, NumberTest) {
    lexers::Lexer lex{"5"};
    ASSERT_EQ(Lexer::TokNumber, lex.getTokType());
    ASSERT_EQ(5, lex.getNum());
    ASSERT_EQ(Lexer::TokEOF, lex.getTokType());

    lex.appendExp("-5");
    ASSERT_EQ(Lexer::TokNumber, lex.getTokType());
    ASSERT_EQ(-5, lex.getNum());
    ASSERT_EQ(Lexer::TokEOF, lex.getTokType());

    lex.appendExp("+5");
    ASSERT_EQ(Lexer::TokNumber, lex.getTokType());
    ASSERT_EQ(5, lex.getNum());
    ASSERT_EQ(Lexer::TokEOF, lex.getTokType());
}

TEST(LexersTest, IdentifierTest) {
    lexers::Lexer lex{"abs"};
    ASSERT_EQ(Lexer::TokIdentifier, lex.getTokType());
    ASSERT_STREQ("abs", lex.getIdentifier().c_str());
    ASSERT_EQ(Lexer::TokEOF, lex.getTokType());
}

TEST(LexersTest, BraceTest) {
    lexers::Lexer lex{"(abs 5)"};
    ASSERT_EQ(Lexer::TokOpeningBracket, lex.getTokType());
    ASSERT_EQ(Lexer::TokIdentifier, lex.stepForward());
    ASSERT_STREQ("abs", lex.getIdentifier().c_str());
    ASSERT_EQ(Lexer::TokNumber, lex.getTokType());
    ASSERT_EQ(5, lex.getNum());
    ASSERT_EQ(Lexer::TokClosingBracket, lex.getTokType());
    ASSERT_EQ(Lexer::TokEOF, lex.stepForward());
}

TEST(LexersTest, DefineTest) {
    lexers::Lexer lex{"(define n 5)"};
    ASSERT_EQ(Lexer::TokOpeningBracket, lex.getTokType());
    ASSERT_EQ(Lexer::TokDefine, lex.stepForward());
    ASSERT_EQ(Lexer::TokIdentifier, lex.stepForward());
    ASSERT_STREQ("n", lex.getIdentifier().c_str());
    ASSERT_EQ(Lexer::TokNumber, lex.getTokType());
    ASSERT_EQ(5, lex.getNum());
    ASSERT_EQ(Lexer::TokClosingBracket, lex.getTokType());
    ASSERT_EQ(Lexer::TokEOF, lex.stepForward());
}

TEST(LexersTest, AliashDefineTest) {
    lexers::Lexer lex{"(define n a)"};
    ASSERT_EQ(Lexer::TokOpeningBracket, lex.getTokType());
    ASSERT_EQ(Lexer::TokDefine, lex.stepForward());
    ASSERT_EQ(Lexer::TokIdentifier, lex.stepForward());
    ASSERT_STREQ("n", lex.getIdentifier().c_str());
    ASSERT_EQ(Lexer::TokIdentifier, lex.getTokType());
    ASSERT_STREQ("a", lex.getIdentifier().c_str());
    ASSERT_EQ(Lexer::TokClosingBracket, lex.getTokType());
    ASSERT_EQ(Lexer::TokEOF, lex.stepForward());
}

TEST(LexersTest, LetTest) {
    lexers::Lexer lex{"(let n 5)"};
    ASSERT_EQ(Lexer::TokOpeningBracket, lex.getTokType());
    ASSERT_EQ(Lexer::TokLet, lex.stepForward());
    ASSERT_EQ(Lexer::TokIdentifier, lex.stepForward());
    ASSERT_STREQ("n", lex.getIdentifier().c_str());
    ASSERT_EQ(Lexer::TokNumber, lex.getTokType());
    ASSERT_EQ(5, lex.getNum());
    ASSERT_EQ(Lexer::TokClosingBracket, lex.getTokType());
    ASSERT_EQ(Lexer::TokEOF, lex.stepForward());
}

TEST(LexersTest, AppendTest) {
    lexers::Lexer lex{"(let n 5)"};
    ASSERT_EQ(Lexer::TokOpeningBracket, lex.getTokType());
    ASSERT_EQ(Lexer::TokLet, lex.stepForward());
    ASSERT_EQ(Lexer::TokIdentifier, lex.stepForward());
    ASSERT_STREQ("n", lex.getIdentifier().c_str());
    ASSERT_EQ(Lexer::TokNumber, lex.getTokType());
    ASSERT_EQ(5, lex.getNum());
    ASSERT_EQ(Lexer::TokClosingBracket, lex.getTokType());

    lex.appendExp("(define n a)");
    ASSERT_EQ(Lexer::TokOpeningBracket, lex.stepForward());
    ASSERT_EQ(Lexer::TokDefine, lex.stepForward());
    ASSERT_EQ(Lexer::TokIdentifier, lex.stepForward());
    ASSERT_STREQ("n", lex.getIdentifier().c_str());
    ASSERT_EQ(Lexer::TokIdentifier, lex.getTokType());
    ASSERT_STREQ("a", lex.getIdentifier().c_str());
    ASSERT_EQ(Lexer::TokClosingBracket, lex.getTokType());
    ASSERT_EQ(Lexer::TokEOF, lex.stepForward());
}

