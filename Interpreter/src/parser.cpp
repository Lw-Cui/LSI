#include <memory>
#include <lexers.h>
#include <parser.h>

using namespace lexers;
using namespace parser;
using namespace std;

shared_ptr<ExprAST> parser::parseExpr(lexers::Lexer &lex) {
    CLOG(DEBUG, "parser") << "Parse expression";
    if (lex.getTokType() != Lexer::TokOpeningBracket) {
        CLOG(DEBUG, "parser") << "Lexer Token type" << lex.getTokType();
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
    } else {
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
    CLOG(DEBUG, "parser");
    return make_shared<NumberAST>(lex.getNum());
}

shared_ptr<ExprAST> parser::parseIdentifierExpr(lexers::Lexer &lex) {
    CLOG(DEBUG, "parser");
    return make_shared<IdentifierAST>(lex.getIdentifier());
}

shared_ptr<ExprAST> parser::parseFunctionApplicationExpr(lexers::Lexer &lex) {
    string identifier = lex.getIdentifier();
    CLOG(DEBUG, "parser") << "Parse function: " << identifier;
    vector<shared_ptr<ExprAST>> arguments;
    while (lex.getTokType() != Lexer::TokClosingBracket) {
        CLOG(DEBUG, "parser") << "Parse arguments: " << lex.getTokType();
        arguments.push_back(parseExpr(lex));
    }
    CLOG(DEBUG, "parser") << "End parsing arguments";
    return make_shared<FunctionApplicationAST>(identifier, arguments);
}

shared_ptr<ExprAST> parser::parseLambdaApplicationExpr(lexers::Lexer &lex) {
    auto lambda = parseExpr(lex);
    CLOG(DEBUG, "parser") << "After parsing lambda, the token is " << lex.getTokType();
    vector<shared_ptr<ExprAST>> arguments;
    CLOG(DEBUG, "parser") << "Parsing lambda application arguments";
    while (lex.getTokType() != Lexer::TokClosingBracket) {
        CLOG(DEBUG, "parser") << "Parse arguments: " << lex.getTokType();
        arguments.push_back(parseExpr(lex));
    }
    return make_shared<LambdaApplicationAST>(lambda, arguments);
}

shared_ptr<ExprAST> parser::parseIdDefinitionExpr(lexers::Lexer &lex) {
    auto identifier = lex.getIdentifier();
    CLOG(DEBUG, "parser") << "define identifier: " << identifier;
    return make_shared<ValueBindingAST>(identifier, parseExpr(lex));
}

shared_ptr<ExprAST> parser::parseFunctionDefinitionExpr(lexers::Lexer &lex) {
    if (lex.stepForward() != Lexer::TokIdentifier) {
        CLOG(DEBUG, "exception");
        throw logic_error("Token isn't open brace during parsing function definition.");
    }

    auto identifier = lex.getIdentifier();
    CLOG(DEBUG, "parser") << "Define function: " << identifier;

    vector<string> args;
    while (lex.getTokType() == Lexer::TokIdentifier) {
        CLOG(DEBUG, "parser") << "Read formal argument";
        args.push_back(lex.getIdentifier());
    }

    if (lex.getTokType() != Lexer::TokClosingBracket) {
        CLOG(DEBUG, "exception");
        throw logic_error("Token cannot end arguments parsing.");
    } else {
        lex.stepForward();
    }

    auto expr = parseExpr(lex);
    if (lex.getTokType() != Lexer::TokClosingBracket) {
        CLOG(DEBUG, "exception");
        throw logic_error("Token cannot end function parsing");
    }

    return make_shared<LambdaBindingAST>(identifier, args, expr);
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
}
