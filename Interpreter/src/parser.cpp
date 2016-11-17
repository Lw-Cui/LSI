#include <memory>
#include <fstream>
#include <lexers.h>
#include <parser.h>


using namespace lexers;
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
            CLOG(DEBUG, "parser") << "Parse number";
            return parseNumberExpr(lex);
        case Lexer::TokIdentifier:
            CLOG(DEBUG, "parser") << "Parse identifier";
            return parseIdentifierExpr(lex);
        case Lexer::TokTrue:
            CLOG(DEBUG, "parser") << "Parse #t";
            return parseTrueExpr(lex);
        case Lexer::TokFalse:
            CLOG(DEBUG, "parser") << "Parse #f";
            return parseFalseExpr(lex);
        default:
            CLOG(DEBUG, "exception");
            throw logic_error("Cannot parse number/identifier.");
    }
}

std::shared_ptr<ExprAST> parser::parseBracketExpr(lexers::Lexer &lex) {
    shared_ptr<ExprAST> res;
    switch (lex.stepForward()) { // eat open brace
        case Lexer::TokOpeningBracket:
            res = parseLambdaApplicationExpr(lex);
            break;
        case Lexer::TokIdentifier:
            CLOG(DEBUG, "parser") << "Parse function Call";
            res = parseFunctionApplicationExpr(lex);
            CLOG(DEBUG, "parser") << "End parsing function Call";
            break;
        case Lexer::TokDefine:
            CLOG(DEBUG, "parser") << "Parse Definition";
            res = parseDefinitionExpr(lex);
            break;
        case Lexer::TokOperator:
            CLOG(DEBUG, "parser") << "Parse Operator";
            res = parseOperatorExpr(lex);
            break;
        case Lexer::TokIf:
            CLOG(DEBUG, "parser") << "Parse If statement";
            res = parseIfStatementExpr(lex);
            break;
        case Lexer::TokLoad:
            CLOG(DEBUG, "parser") << "Parse loading statement";
            res = parseLoadingFileExpr(lex);
            break;
        case Lexer::TokLambda:
            CLOG(DEBUG, "parser") << "Parse lambda Definition";
            res = parseLambdaDefinitionExpr(lex);
            break;
        default:
            CLOG(DEBUG, "exception");
            throw logic_error("Cannot parse token.");
    }
    if (lex.getTokType() != Lexer::TokClosingBracket) {
        CLOG(DEBUG, "exception");
        throw ("Format error: Token isn't close brace during parsing expression.");
    } else {
        lex.stepForward(); // eat close brace
    }
    return std::move(res);
}


std::shared_ptr<ExprAST> parser::parseOperatorExpr(lexers::Lexer &lex) {
    switch (lex.getOperator()[0]) {
        case '+':
            CLOG(DEBUG, "parser") << "parsing add operator";
            return parseAddOperatorExpr(lex);
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

std::shared_ptr<ExprAST> parser::parseLessThanOperatorExpr(lexers::Lexer &lex) {
    std::vector<std::shared_ptr<ExprAST>> arguments;
    while (lex.getTokType() != Lexer::TokClosingBracket)
        arguments.push_back(parseExpr(lex));
    CLOG(DEBUG, "parser") << "Number of less than operands are: " << arguments.size();
    return make_shared<LessThanOperatorAST>(arguments);
}

shared_ptr<ExprAST> parser::parseNumberExpr(Lexer &lex) {
    return make_shared<NumberAST>(lex.getNum());
}

shared_ptr<ExprAST> parser::parseIdentifierExpr(lexers::Lexer &lex) {
    return make_shared<IdentifierAST>(lex.getIdentifier());
}


shared_ptr<ExprAST> parser::parseIdDefinitionExpr(lexers::Lexer &lex) {
    auto identifier = lex.getIdentifier();
    CLOG(DEBUG, "parser") << "define identifier: " << identifier;
    return make_shared<ValueBindingAST>(identifier, parseExpr(lex));
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

