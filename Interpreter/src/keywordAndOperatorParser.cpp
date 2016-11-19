#include <memory>
#include <fstream>
#include <lexers.h>
#include <basicParser.h>

using namespace lexers;
using namespace parser;
using namespace std;

std::shared_ptr<ExprAST> parser::parseOperatorExpr(lexers::Lexer &lex) {
    auto op = lex.getOperator();
    switch (op.front()) {
        case '+':
            CLOG(DEBUG, "parser") << "parsing add operator";
            return parseAddOperatorExpr(lex);
        case '-':
            CLOG(DEBUG, "parser") << "parsing minus operator";
            return parseMinusOperatorExpr(lex);
        case '<':
            CLOG(DEBUG, "parser") << "parsing less than operator";
            return parseLessThanOperatorExpr(lex);
        default:
            CLOG(DEBUG, "exception");
            throw logic_error("Cannot parse operator.");
    }
}

std::shared_ptr<ExprAST> parser::parseAddOperatorExpr(lexers::Lexer &lex) {
    std::vector<std::shared_ptr<ExprAST>> arguments;
    while (lex.getTokType() != Lexer::TokClosingBracket)
        arguments.push_back(parseExpr(lex));
    CLOG(DEBUG, "parser") << "Number of add operands are: " << arguments.size();
    return make_shared<AddOperatorAST>(arguments);
}

std::shared_ptr<ExprAST> parser::parseMinusOperatorExpr(lexers::Lexer &lex) {
    std::vector<std::shared_ptr<ExprAST>> arguments;
    while (lex.getTokType() != Lexer::TokClosingBracket)
        arguments.push_back(parseExpr(lex));
    CLOG(DEBUG, "parser") << "Number of minus operands are: " << arguments.size();
    return make_shared<MinusOperatorAST>(arguments);
}

std::shared_ptr<ExprAST> parser::parseLessThanOperatorExpr(lexers::Lexer &lex) {
    std::vector<std::shared_ptr<ExprAST>> arguments;
    while (lex.getTokType() != Lexer::TokClosingBracket)
        arguments.push_back(parseExpr(lex));
    CLOG(DEBUG, "parser") << "Number of less than operands are: " << arguments.size();
    return make_shared<LessThanOperatorAST>(arguments);
}

shared_ptr<ExprAST> parser::parseDefinitionExpr(lexers::Lexer &lex) {
    switch (lex.stepForward()) {
        case Lexer::TokIdentifier:
            CLOG(DEBUG, "parser") << "Parse identifier Definition";
            return parseIdDefinitionExpr(lex);
        case Lexer::TokOpeningBracket:
            return parseFunctionDefinitionExpr(lex);
        default:
            CLOG(DEBUG, "exception");
            throw logic_error("Cannot parse definition.");
    }

}

std::shared_ptr<ExprAST> parser::parseIfStatementExpr(lexers::Lexer &lex) {
    lex.stepForward();
    auto condition = parseExpr(lex);
    CLOG(DEBUG, "parser") << "parsing true clause";
    auto trueClause = parseExpr(lex);
    CLOG(DEBUG, "parser") << "parsing false clause";
    auto falseClause = parseExpr(lex);
    CLOG(DEBUG, "parser") << "Finished if statement parsing";
    return make_shared<IfStatementAST>(condition, trueClause, falseClause);
}

std::shared_ptr<ExprAST> parser::parseTrueExpr(lexers::Lexer &lex) {
    lex.stepForward();
    return make_shared<BooleansAST>(true);
}

std::shared_ptr<ExprAST> parser::parseFalseExpr(lexers::Lexer &lex) {
    lex.stepForward();
    return make_shared<BooleansAST>(false);
}

std::shared_ptr<ExprAST> parser::parseLoadingFileExpr(lexers::Lexer &lex) {
    lex.stepForward();
    auto filename = lex.getIdentifier().substr(1);
    filename.pop_back();
    CLOG(DEBUG, "parser") << "Get filename: " << filename;
    return make_shared<LoadingFileAST>(filename);
}

std::shared_ptr<ExprAST> parser::parseNilExpr(lexers::Lexer &lex) {
    lex.stepForward();
    return std::make_shared<NilAST>();
}




