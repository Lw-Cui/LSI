#include <memory>
#include <fstream>
#include <lexers.h>
#include <parser.h>

using namespace lexers;
using namespace parser;
using namespace std;


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
    return make_shared<BooleansTrueAST>();
}

std::shared_ptr<ExprAST> parser::parseFalseExpr(lexers::Lexer &lex) {
    lex.stepForward();
    return make_shared<BooleansFalseAST>();
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




