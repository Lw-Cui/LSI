#include <memory>
#include <fstream>
#include <lexers.h>
#include <parser.h>
#include <exception.h>

using namespace lexers;
using namespace parser;
using namespace exception;
using namespace std;


shared_ptr<ExprAST> parser::parseDefinitionExpr(lexers::Lexer &lex) {
    switch (lex.stepForward()) {
        case Lexer::TokIdentifier:
            return parseIdDefinitionExpr(lex);
        case Lexer::TokOpeningBracket:
            return parseFunctionDefinitionExpr(lex);
        default:
            throw UnsupportedSyntax("Cannot parse definition: lexer return invalid type");
    }
}

std::shared_ptr<ExprAST> parser::parseIfStatementExpr(lexers::Lexer &lex) {
    lex.stepForward();
    try {
        auto condition = parseExpr(lex);
        auto trueClause = parseExpr(lex);
        auto falseClause = parseExpr(lex);
        return make_shared<IfStatementAST>(condition, trueClause, falseClause);
    } catch (RuntimeError &e) {
        throw NotAtomType(std::string(e.what()) + " @ if statement");
    }
}

std::shared_ptr<ExprAST> parser::parseTrueExpr(lexers::Lexer &lex) {
    lex.stepForward();
    return make_shared<BooleansTrueAST>();
}

std::shared_ptr<ExprAST> parser::parseCondStatementExpr(lexers::Lexer &lex) {
    lex.stepForward();
    std::vector<std::shared_ptr<ExprAST>> condition, result;
    while (lex.getTokType() != Lexer::TokClosingBracket) {
        lex.stepForward();
        condition.push_back(parseExpr(lex));
        result.push_back(parseExpr(lex));
        lex.stepForward();
    }
    return make_shared<CondStatementAST>(condition, result);
}

std::shared_ptr<ExprAST> parser::parseFalseExpr(lexers::Lexer &lex) {
    lex.stepForward();
    return make_shared<BooleansFalseAST>();
}

std::shared_ptr<ExprAST> parser::parseLoadingFileExpr(lexers::Lexer &lex) {
    lex.stepForward();
    auto filename = lex.getIdentifier().substr(1);
    filename.pop_back();
    return make_shared<LoadingFileAST>(filename);
}

std::shared_ptr<ExprAST> parser::parseNilExpr(lexers::Lexer &lex) {
    lex.stepForward();
    return std::make_shared<NilAST>();
}

std::shared_ptr<ExprAST> parser::parseLetStatementExpr(lexers::Lexer &lex) {
    lex.stepForward();
    lex.stepForward();
    std::vector<std::shared_ptr<ExprAST>> id, v;
    while (lex.getTokType() != Lexer::TokClosingBracket) {
        lex.stepForward();
        id.push_back(parseExpr(lex));
        v.push_back(parseExpr(lex));
        lex.stepForward();
    }
    lex.stepForward();
    std::shared_ptr<ExprAST> expr = parseExpr(lex);
    return make_shared<LetStatementAST>(id, v, expr);
}

