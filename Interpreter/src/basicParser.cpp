#include <memory>
#include <fstream>
#include <lexers.h>
#include <parser.h>
#include <exception.h>

using namespace lexers;
using namespace exception;
using namespace parser;
using namespace std;

std::shared_ptr<ExprAST> parser::parseAllExpr(lexers::Lexer &lex) {
    vector<shared_ptr<ExprAST>> vec;
    while (lex.getTokType() != Lexer::TokEOF)
        vec.push_back(parseExpr(lex));
    return make_shared<AllExprAST>(vec);
}

shared_ptr<ExprAST> parser::parseExpr(lexers::Lexer &lex) {
    if (lex.getTokType() != Lexer::TokOpeningBracket) {
        return parseRawExpr(lex);
    } else {
        return parseBracketExpr(lex);
    }
}

std::shared_ptr<ExprAST> parser::parseRawExpr(lexers::Lexer &lex) {
    switch (lex.getTokType()) {
        case Lexer::TokNumber:
            return parseNumberExpr(lex);
        case Lexer::TokIdentifier:
            return parseIdentifierExpr(lex);
        case Lexer::TokTrue:
            return parseTrueExpr(lex);
        case Lexer::TokFalse:
            return parseFalseExpr(lex);
        case Lexer::TokNil:
            return parseNilExpr(lex);
        default:
            CLOG(DEBUG, "exception");
            throw RawExprError("Cannot parse number/identifier/#f/#t/nil");
    }
}

std::shared_ptr<ExprAST> parser::parseBracketExpr(lexers::Lexer &lex) {
    shared_ptr<ExprAST> res;
    switch (lex.stepForward()) { // eat open brace
        case Lexer::TokOpeningBracket:
            res = parseLambdaApplicationExpr(lex);
            break;
        case Lexer::TokIdentifier:
            res = parseFunctionApplicationExpr(lex);
            break;
        case Lexer::TokDefine:
            res = parseDefinitionExpr(lex);
            break;
        case Lexer::TokIf:
            res = parseIfStatementExpr(lex);
            break;
        case Lexer::TokCond:
            res = parseCondStatementExpr(lex);
            break;
        case Lexer::TokLet:
            res = parseLetStatementExpr(lex);
            break;
        case Lexer::TokLoad:
            res = parseLoadingFileExpr(lex);
            break;
        case Lexer::TokLambda:
            res = parseLambdaDefinitionExpr(lex);
            break;
        default:
            CLOG(DEBUG, "exception");
            throw logic_error("Cannot parse token.");
    }
    if (lex.getTokType() != Lexer::TokClosingBracket) {
        CLOG(DEBUG, "exception");
        throw BracketMatchError("Bracket doesn't match");
    } else {
        lex.stepForward(); // eat close brace
    }
    return std::move(res);
}


shared_ptr<ExprAST> parser::parseNumberExpr(Lexer &lex) {
    auto num = lex.getNum();
    CLOG(DEBUG, "parser") << "Parse number " << num;
    return make_shared<NumberAST>(num);
}

shared_ptr<ExprAST> parser::parseIdentifierExpr(lexers::Lexer &lex) {
    auto str = lex.getIdentifier();
    CLOG(DEBUG, "parser") << "Parse identifier " << str;
    return make_shared<IdentifierAST>(str);
}


shared_ptr<ExprAST> parser::parseIdDefinitionExpr(lexers::Lexer &lex) {
    auto identifier = lex.getIdentifier();
    CLOG(DEBUG, "parser") << "Parse identifier Definition: " << identifier;
    return make_shared<ValueBindingAST>(identifier, parseExpr(lex));
}


