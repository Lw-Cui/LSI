#include <memory>
#include <lexers.h>
#include <parser.h>
#include <easylogging++.h>

using namespace lexers;
using namespace parser;
using namespace std;

std::shared_ptr<ExprAST> parser::parseExpr(lexers::Lexer &lex) {
    if (lex.getTokType() != Lexer::TokOpenBrace) {
        switch (lex.getTokType()) {
            case Lexer::TokNumber:
                LOG(DEBUG) << "Parse number";
                return parseNumberExpr(lex);
            case Lexer::TokIdentifier:
                LOG(DEBUG) << "Parse identifier";
                return parseIdentifierExpr(lex);
            default:
                throw logic_error("Cannot parse number/identifier.");
        }
    } else {
        shared_ptr<ExprAST> res;
        switch (lex.getNextTok()) { // eat open brace
            case Lexer::TokOpenBrace:
                res = parseFunctionCallExpr(lex);
                break;
            case Lexer::TokIdentifier:
                res = parseFunctionCallExpr(lex);
                break;
            case Lexer::TokLet:
                res = parseLetExpr(lex);
                break;
            case Lexer::TokDefine:
                LOG(DEBUG) << "Parse Definition";
                res = parseDefinitionExpr(lex);
                break;
            default:
                throw logic_error("Cannot parse token.");
        }
        lex.getNextTok(); // eat close brace
        return std::move(res);
    }
}

shared_ptr<ExprAST> parser::parseNumberExpr(Lexer &lex) {
    LOG(DEBUG);
    return make_shared<NumberAST>(lex.getNum());
}

shared_ptr<ExprAST> parser::parseIdentifierExpr(lexers::Lexer &lex) {
    LOG(DEBUG);
    return make_shared<IdentifierAST>(lex.getIdentifier());
}

shared_ptr<ExprAST> parser::parseFunctionCallExpr(lexers::Lexer &lex) {
}

shared_ptr<ExprAST> parser::parseLetExpr(lexers::Lexer &lex) {
}

shared_ptr<ExprAST> parser::parseIdDefinitionExpr(lexers::Lexer &lex) {
    auto identifier = lex.getIdentifier();
    LOG(DEBUG) << "define identifier: " << identifier;
    return make_shared<IdentifierDefinitionAST>(make_shared<IdentifierAST>(identifier), parseExpr(lex));
}

shared_ptr<ExprAST> parser::parseFunctionDefinitionExpr(lexers::Lexer &lex) {
}

shared_ptr<ExprAST> parser::parseDefinitionExpr(lexers::Lexer &lex) {
    switch (lex.getNextTok()) {
        case Lexer::TokIdentifier:
                LOG(DEBUG) << "Parse identifier Definition";
            return parseIdDefinitionExpr(lex);
        case Lexer::TokOpenBrace:
            return parseFunctionDefinitionExpr(lex);
        default:
            throw logic_error("Cannot parse definition.");
    }
}
