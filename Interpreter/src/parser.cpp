#include <memory>
#include <lexers.h>
#include <parser.h>

using namespace lexers;
using namespace parser;
using namespace std;

std::shared_ptr<ExprAST> parser::parseExpr(lexers::Lexer &lex) {
    if (lex.getTokType() != Lexer::TokOpenBrace) {
        switch (lex.getTokType()) {
            case Lexer::TokNumber:
                return parseNumberExpr(lex);
            case Lexer::TokIdentifier:
                return parseIdentifierExpr(lex);
            default:
                throw logic_error("Cannot parse number/identifier.");
        }
    } else {
        switch (lex.getNextTok()) { // eat open brace
            case Lexer::TokOpenBrace:
                return parseFunctionCallExpr(lex);
            case Lexer::TokIdentifier:
                return parseFunctionCallExpr(lex);
            case Lexer::TokLet:
                return parseLetExpr(lex);
            case Lexer::TokDefine:
                return parseDefinitionExpr(lex);
            default:
                throw logic_error("Cannot parse token.");
        }
    }
}

shared_ptr<ExprAST> parser::parseNumberExpr(Lexer &lex) {
    return make_shared<NumberAST>(lex.getNum());
}

shared_ptr<ExprAST> parser::parseIdentifierExpr(lexers::Lexer &lex) {
    return make_shared<IdentifierAST>(lex.getIdentifier());
}

shared_ptr<ExprAST> parser::parseFunctionCallExpr(lexers::Lexer &lex) {
}

shared_ptr<ExprAST> parser::parseLetExpr(lexers::Lexer &lex) {
}

shared_ptr<ExprAST> parser::parseIdDefinitionExpr(lexers::Lexer &lex) {
}

shared_ptr<ExprAST> parser::parseFunctionDefinitionExpr(lexers::Lexer &lex) {
}

shared_ptr<ExprAST> parser::parseDefinitionExpr(lexers::Lexer &lex) {
    switch (lex.getNextTok()) {
        case Lexer::TokIdentifier:
            return parseIdDefinitionExpr(lex);
        case Lexer::TokOpenBrace:
            return parseFunctionDefinitionExpr(lex);
        default:
            throw logic_error("Cannot parse definition.");
    }
}
