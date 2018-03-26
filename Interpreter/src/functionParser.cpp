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
    vector<shared_ptr<ExprAST>> arguments;
    while (lex.getTokType() != Lexer::TokClosingBracket) {
        arguments.push_back(parseExpr(lex));
    }
    return make_shared<InvocationAST>(make_shared<IdentifierAST>(identifier), arguments);
}

shared_ptr<ExprAST> parser::parseLambdaApplicationExpr(lexers::Lexer &lex) {
    auto lambda = parseExpr(lex);
    vector<shared_ptr<ExprAST>> arguments;
    while (lex.getTokType() != Lexer::TokClosingBracket) {
        arguments.push_back(parseExpr(lex));
    }
    return make_shared<InvocationAST>(lambda, arguments);
}

std::shared_ptr<ExprAST> parser::parseLambdaDefinitionExpr(lexers::Lexer &lex) {
    if (lex.stepForward() != Lexer::TokOpeningBracket || lex.stepForward() != Lexer::TokIdentifier) {
        throw UnsupportedSyntax("Lambda definition needs argument(s)");
    }
    vector<string> args;
    while (lex.getTokType() == Lexer::TokIdentifier) {
        args.push_back(lex.getIdentifier());
    }

    if (lex.getTokType() != Lexer::TokClosingBracket) {
        throw MissBracket("Lambda definition need ) to end argument declaration");
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
        throw UnsupportedSyntax("Function definition needs argument(s)");
    }

    auto identifier = lex.getIdentifier();
    vector<string> args;
    while (lex.getTokType() == Lexer::TokIdentifier) {
        args.push_back(lex.getIdentifier());
    }

    if (lex.getTokType() != Lexer::TokClosingBracket) {
        throw MissBracket("Function definition need ) to end argument declaration");
    } else {
        lex.stepForward();
    }

    std::vector<std::shared_ptr<ExprAST>> expression;
    while (lex.getTokType() != Lexer::TokClosingBracket) {
        expression.push_back(parseExpr(lex));
    }
    return make_shared<LambdaBindingAST>(identifier, args, expression);
}

