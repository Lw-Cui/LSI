#include <gtest/gtest.h>
#include <parser.h>

using namespace lexers;

TEST(LexersTest, NumberTest) {
    lexers::Lexer lex{"5"};
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
    ASSERT_EQ(Lexer::TokOpenBrace, lex.getTokType());
    ASSERT_EQ(Lexer::TokIdentifier, lex.getNextTok());
    ASSERT_STREQ("abs", lex.getIdentifier().c_str());
    ASSERT_EQ(Lexer::TokNumber, lex.getTokType());
    ASSERT_EQ(5, lex.getNum());
    ASSERT_EQ(Lexer::TokCloseBrace, lex.getTokType());
    ASSERT_EQ(Lexer::TokEOF, lex.getNextTok());
}

TEST(LexersTest, DefineTest) {
    lexers::Lexer lex{"(define n 5)"};
    ASSERT_EQ(Lexer::TokOpenBrace, lex.getTokType());
    ASSERT_EQ(Lexer::TokDefine, lex.getNextTok());
    ASSERT_EQ(Lexer::TokIdentifier, lex.getNextTok());
    ASSERT_STREQ("n", lex.getIdentifier().c_str());
    ASSERT_EQ(Lexer::TokNumber, lex.getTokType());
    ASSERT_EQ(5, lex.getNum());
    ASSERT_EQ(Lexer::TokCloseBrace, lex.getTokType());
    ASSERT_EQ(Lexer::TokEOF, lex.getNextTok());
}

TEST(LexersTest, AliashDefineTest) {
    lexers::Lexer lex{"(define n a)"};
    ASSERT_EQ(Lexer::TokOpenBrace, lex.getTokType());
    ASSERT_EQ(Lexer::TokDefine, lex.getNextTok());
    ASSERT_EQ(Lexer::TokIdentifier, lex.getNextTok());
    ASSERT_STREQ("n", lex.getIdentifier().c_str());
    ASSERT_EQ(Lexer::TokIdentifier, lex.getTokType());
    ASSERT_STREQ("a", lex.getIdentifier().c_str());
    ASSERT_EQ(Lexer::TokCloseBrace, lex.getTokType());
    ASSERT_EQ(Lexer::TokEOF, lex.getNextTok());
}

TEST(LexersTest, LetTest) {
    lexers::Lexer lex{"(let n 5)"};
    ASSERT_EQ(Lexer::TokOpenBrace, lex.getTokType());
    ASSERT_EQ(Lexer::TokLet, lex.getNextTok());
    ASSERT_EQ(Lexer::TokIdentifier, lex.getNextTok());
    ASSERT_STREQ("n", lex.getIdentifier().c_str());
    ASSERT_EQ(Lexer::TokNumber, lex.getTokType());
    ASSERT_EQ(5, lex.getNum());
    ASSERT_EQ(Lexer::TokCloseBrace, lex.getTokType());
    ASSERT_EQ(Lexer::TokEOF, lex.getNextTok());
}

TEST(LexerTest, AppendTest) {
    lexers::Lexer lex{"(let n 5)"};
    ASSERT_EQ(Lexer::TokOpenBrace, lex.getTokType());
    ASSERT_EQ(Lexer::TokLet, lex.getNextTok());
    ASSERT_EQ(Lexer::TokIdentifier, lex.getNextTok());
    ASSERT_STREQ("n", lex.getIdentifier().c_str());
    ASSERT_EQ(Lexer::TokNumber, lex.getTokType());
    ASSERT_EQ(5, lex.getNum());
    ASSERT_EQ(Lexer::TokCloseBrace, lex.getTokType());

    lex.appendExp("(define n a)");
    ASSERT_EQ(Lexer::TokOpenBrace, lex.getNextTok());
    ASSERT_EQ(Lexer::TokDefine, lex.getNextTok());
    ASSERT_EQ(Lexer::TokIdentifier, lex.getNextTok());
    ASSERT_STREQ("n", lex.getIdentifier().c_str());
    ASSERT_EQ(Lexer::TokIdentifier, lex.getTokType());
    ASSERT_STREQ("a", lex.getIdentifier().c_str());
    ASSERT_EQ(Lexer::TokCloseBrace, lex.getTokType());
    ASSERT_EQ(Lexer::TokEOF, lex.getNextTok());
}

TEST(LexersTest, OperatorTest) {
    lexers::Lexer lex{"(+ n 5)"};
    ASSERT_EQ(Lexer::TokOpenBrace, lex.getTokType());
    ASSERT_EQ(Lexer::TokOperator, lex.getNextTok());
    ASSERT_EQ(Lexer::TokIdentifier, lex.getNextTok());
    ASSERT_STREQ("n", lex.getIdentifier().c_str());
    ASSERT_EQ(Lexer::TokNumber, lex.getTokType());
    ASSERT_EQ(5, lex.getNum());
    ASSERT_EQ(Lexer::TokCloseBrace, lex.getTokType());
    ASSERT_EQ(Lexer::TokEOF, lex.getNextTok());
}
