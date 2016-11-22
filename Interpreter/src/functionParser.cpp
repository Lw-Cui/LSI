#include <memory>
#include <fstream>
#include <lexers.h>
#include <parser.h>

using namespace lexers;
using namespace parser;
using namespace std;

shared_ptr<ExprAST> parser::parseFunctionApplicationExpr(lexers::Lexer &lex) {
    string identifier = lex.getIdentifier();
    CLOG(DEBUG, "parser") << "Parse function: " << identifier;
    vector<shared_ptr<ExprAST>> arguments;
    while (lex.getTokType() != Lexer::TokClosingBracket) {
        CLOG(DEBUG, "parser") << "Parse arguments: " << lex.getTokType();
        arguments.push_back(parseExpr(lex));
    }
    CLOG(DEBUG, "parser") << "End parsing arguments";
    return make_shared<LambdaApplicationAST>(make_shared<IdentifierAST>(identifier), arguments);
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

std::shared_ptr<ExprAST> parser::parseLambdaDefinitionExpr(lexers::Lexer &lex) {
    if (lex.stepForward() != Lexer::TokOpeningBracket || lex.stepForward() != Lexer::TokIdentifier) {
        CLOG(DEBUG, "exception") << lex.getTokType();
        throw logic_error("Lambda definition error.");
    }
    vector<string> args;
    while (lex.getTokType() == Lexer::TokIdentifier) {
        CLOG(DEBUG, "parser") << "Read formal argument";
        args.push_back(lex.getIdentifier());
    }

    if (lex.getTokType() != Lexer::TokClosingBracket) {
        CLOG(DEBUG, "exception");
        throw logic_error("Token cannot end lambda arguments parsing.");
    } else {
        lex.stepForward();
    }

    std::vector<std::shared_ptr<ExprAST>> nestedFunc;
    std::shared_ptr<ExprAST> expr;

    while (true) {
        expr = parseExpr(lex);
        if (lex.getTokType() != Lexer::TokClosingBracket) nestedFunc.push_back(expr);
        else break;
    }
    return make_shared<LambdaAST>(args, expr, nestedFunc);
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

    std::vector<std::shared_ptr<ExprAST>> nestedFunc;
    std::shared_ptr<ExprAST> expr;

    while (true) {
        expr = parseExpr(lex);
        if (lex.getTokType() != Lexer::TokClosingBracket) nestedFunc.push_back(expr);
        else break;
    }

    return make_shared<LambdaBindingAST>(identifier, args, expr, nestedFunc);
}

