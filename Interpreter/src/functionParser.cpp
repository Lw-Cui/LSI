#include <memory>
#include <fstream>
#include <lexers.h>
#include <exception.h>
#include <parser.h>

using namespace lexers;
using namespace exception;
using namespace parser;
using namespace std;

shared_ptr<ExprAST> parser::parseFunctionApplicationExpr(lexers::Lexer &lex) {
    string identifier = lex.getIdentifier();
    CLOG(DEBUG, "parser") << "Parse function Call: " << identifier;
    vector<shared_ptr<ExprAST>> arguments;
    while (lex.getTokType() != Lexer::TokClosingBracket) {
        arguments.push_back(parseExpr(lex));
    }
    return make_shared<LambdaApplicationAST>(make_shared<IdentifierAST>(identifier), arguments);
}

shared_ptr<ExprAST> parser::parseLambdaApplicationExpr(lexers::Lexer &lex) {
    auto lambda = parseExpr(lex);
    vector<shared_ptr<ExprAST>> arguments;
    while (lex.getTokType() != Lexer::TokClosingBracket) {
        arguments.push_back(parseExpr(lex));
    }
    return make_shared<LambdaApplicationAST>(lambda, arguments);
}

std::shared_ptr<ExprAST> parser::parseLambdaDefinitionExpr(lexers::Lexer &lex) {
    CLOG(DEBUG, "parser") << "Parse lambda Definition";
    if (lex.stepForward() != Lexer::TokOpeningBracket || lex.stepForward() != Lexer::TokIdentifier) {
        CLOG(DEBUG, "exception") << lex.getTokType();
        throw UnexpectedType("Lambda arguments type doesn't match.");
    }
    vector<string> args;
    while (lex.getTokType() == Lexer::TokIdentifier) {
        args.push_back(lex.getIdentifier());
    }

    if (lex.getTokType() != Lexer::TokClosingBracket) {
        CLOG(DEBUG, "exception");
        throw logic_error("Token cannot end lambda arguments parsing.");
    } else {
        lex.stepForward();
    }

    std::vector<std::shared_ptr<ExprAST>> expression;
    while (lex.getTokType() != Lexer::TokClosingBracket) {
        expression.push_back(parseExpr(lex));
    }
    return make_shared<LambdaAST>(args, expression);
}

shared_ptr<ExprAST> parser::parseFunctionDefinitionExpr(lexers::Lexer &lex) {
    if (lex.stepForward() != Lexer::TokIdentifier) {
        CLOG(DEBUG, "exception");
        throw logic_error("Token isn't open brace during parsing function definition.");
    }

    auto identifier = lex.getIdentifier();
    vector<string> args;
    while (lex.getTokType() == Lexer::TokIdentifier) {
        args.push_back(lex.getIdentifier());
    }

    if (lex.getTokType() != Lexer::TokClosingBracket) {
        CLOG(DEBUG, "exception");
        throw logic_error("Token cannot end arguments parsing.");
    } else {
        lex.stepForward();
    }

    std::vector<std::shared_ptr<ExprAST>> expression;
    while (lex.getTokType() != Lexer::TokClosingBracket) {
        expression.push_back(parseExpr(lex));
    }
    return make_shared<LambdaBindingAST>(identifier, args, expression);
}

