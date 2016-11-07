#include <memory>
#include <lexers.h>
#include <parser.h>

using namespace lexers;
using namespace parser;
using namespace std;

shared_ptr<ExprAST> parser::parseExpr(lexers::Lexer &lex) {
    if (lex.getTokType() != Lexer::TokOpenBrace) {
        switch (lex.getTokType()) {
            case Lexer::TokNumber:
                CLOG(DEBUG, "parser") << "Parse number";
                return parseNumberExpr(lex);
            case Lexer::TokIdentifier:
                CLOG(DEBUG, "parser") << "Parse identifier";
                return parseIdentifierExpr(lex);
            default:
                CLOG(DEBUG, "exception");
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
                CLOG(DEBUG, "parser") << "Parse Definition";
                res = parseDefinitionExpr(lex);
                break;
            default:
                CLOG(DEBUG, "exception");
                throw logic_error("Cannot parse token.");
        }
        // eat close brace
        LOG_IF(lex.getNextTok() == Lexer::TokCloseBrace, ERROR)
            << "Format error: Token isn't close brace during parsing expression.";
        return std::move(res);
    }
}

shared_ptr<ExprAST> parser::parseNumberExpr(Lexer &lex) {
    CLOG(DEBUG, "parser");
    return make_shared<NumberAST>(lex.getNum());
}

shared_ptr<ExprAST> parser::parseIdentifierExpr(lexers::Lexer &lex) {
    CLOG(DEBUG, "parser");
    return make_shared<IdentifierAST>(lex.getIdentifier());
}

shared_ptr<ExprAST> parser::parseFunctionCallExpr(lexers::Lexer &lex) {
}

shared_ptr<ExprAST> parser::parseLetExpr(lexers::Lexer &lex) {
}

shared_ptr<ExprAST> parser::parseIdDefinitionExpr(lexers::Lexer &lex) {
    auto identifier = lex.getIdentifier();
    CLOG(DEBUG, "parser") << "define identifier: " << identifier;
    return make_shared<IdentifierDefinitionAST>(make_shared<IdentifierAST>(identifier), parseExpr(lex));
}

shared_ptr<ExprAST> parser::parseFunctionDefinitionExpr(lexers::Lexer &lex) {
    if (lex.getNextTok() != Lexer::TokIdentifier) {
        LOG(ERROR) << "Token isn't open brace during parsing function definition.";
        // throw
    }

    auto identifier = lex.getIdentifier();
    LOG(DEBUG) << "Define function: " << identifier;

    vector<shared_ptr<IdentifierAST>> args;
    while (lex.getNextTok() == Lexer::TokIdentifier) {
        shared_ptr<ExprAST> arg{parseExpr(lex)};
        auto idPtr = dynamic_cast<IdentifierAST *>(arg.get());
        if (!idPtr) {
            LOG(ERROR) << "Cannot convert to identifier.";
            // throw
        }
        args.push_back(make_shared<IdentifierAST>(idPtr->getId()));
    }

    if (lex.getTokType() != Lexer::TokCloseBrace) {
        LOG(ERROR) << "Token isn't close brace during parsing function definition.";
        // throw
    }
    lex.getNextTok();

    auto expr = parseExpr(lex);
    if (lex.getNextTok() != Lexer::TokCloseBrace) {
        LOG(ERROR) << "Token cannot end function parsing";
        // throw
    }

    return make_shared<FunctionDefinitionAST>(identifier, args, expr);
}

shared_ptr<ExprAST> parser::parseDefinitionExpr(lexers::Lexer &lex) {
    switch (lex.getNextTok()) {
        case Lexer::TokIdentifier:
                CLOG(DEBUG, "parser") << "Parse identifier Definition";
            return parseIdDefinitionExpr(lex);
        case Lexer::TokOpenBrace:
            return parseFunctionDefinitionExpr(lex);
        default:
            CLOG(DEBUG, "exception");
            throw logic_error("Cannot parse definition.");
    }
}
